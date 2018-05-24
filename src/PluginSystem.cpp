/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "PluginSystem.hpp"

Plugin::Plugin(size_t id,
                std::string_view identity,
                const fs::path &path)
{
    char errorSPMsg[256];
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    SourcePawn::IPluginRuntime *plugin = spAPIv2->LoadBinaryFromFile(path.string().c_str(),
                                                                     errorSPMsg,
                                                                     sizeof(errorSPMsg));

    if (!plugin)
        throw std::runtime_error(errorSPMsg);

    uint32_t infoVarIndex;
    if (plugin->FindPubvarByName("pluginInfo", &infoVarIndex) != SP_ERROR_NONE)
        throw std::runtime_error("Can't find plugin info!");

    sp_pubvar_t *infoVar;
    plugin->GetPubvarByIndex(infoVarIndex, &infoVar);

    auto gatherInfo = [plugin, infoVar](uint32_t field)
    {
        char *infoField;
        plugin->GetDefaultContext()->LocalToString(*(infoVar->offs + field), &infoField);
        return infoField;
    };

    m_name = gatherInfo(Plugin::FIELD_NAME);
    m_version = gatherInfo(Plugin::FIELD_VERSION);
    m_author = gatherInfo(Plugin::FIELD_AUTHOR);
    m_url = gatherInfo(Plugin::FIELD_URL);

    m_filename = path.filename().string();
    m_id = id;
    m_identity = identity;
    m_runtime = plugin;
    m_runtime->GetDefaultContext()->SetKey(1, const_cast<char *>(m_identity.c_str()));

    uint32_t nativesNum = plugin->GetNativesNum();
    const std::unique_ptr<NativeMngr> &nativeManager = gSPGlobal->getNativeManagerCore();
    for (uint32_t index = 0; index < nativesNum; ++index)
    {
        const sp_native_t *pluginNative = m_runtime->GetNative(index);

        if (pluginNative->status == SP_NATIVE_BOUND)
            continue;

        std::shared_ptr<Native> native = nativeManager->getNativeCore(pluginNative->name);
        if (!native)
            continue;

        plugin->UpdateNativeBinding(index, native->getRouter(), 0, nullptr);
    }

    // Setup maxclients num
    uint32_t maxClientsVarIndex;
    if (plugin->FindPubvarByName("maxClients", &maxClientsVarIndex) == SP_ERROR_NONE)
    {
        cell_t local_addr, *phys_addr;
        plugin->GetPubvarAddrs(maxClientsVarIndex, &local_addr, &phys_addr);
        *phys_addr = gRehldsServerStatic->GetMaxClients();
    }
}

IForward *Plugin::createForward(const char *name,
                                size_t params,
                                ...) const
{
    if (params > SP_MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    va_list paramsList;
    va_start(paramsList, params);
    auto createdForward = _createForwardVa(name, paramsList, params);
    va_end(paramsList);

    return createdForward.get();
}

std::shared_ptr<Forward> Plugin::_createForwardVa(std::string_view name,
                                                  std::va_list paramsList,
                                                  size_t paramsnum) const
{
    fwdParamTypeList forwardParams;

    for (size_t i = 0; i < paramsnum; ++i)
        forwardParams.at(i) = static_cast<Forward::ParamType>(va_arg(paramsList, int));

    return _createForward(name, forwardParams, paramsnum);
}

std::shared_ptr<Forward> Plugin::createForwardCore(std::string_view name,
                                                   fwdInitParamsList params) const
{
    auto paramsNum = params.size();

    if (paramsNum > SP_MAX_EXEC_PARAMS)
        return nullptr;

    fwdParamTypeList forwardParams;
    std::copy(params.begin(), params.end(), forwardParams.begin());

    return _createForward(name, forwardParams, paramsNum);
}

std::shared_ptr<Forward> Plugin::_createForward(std::string_view name,
                                                fwdParamTypeList paramlist,
                                                size_t paramsnum) const
{
    auto plugin = gSPGlobal->getPluginManagerCore()->getPluginCore(m_identity);
    auto forwardPtr = std::make_shared<Forward>(name, paramlist, paramsnum, plugin);

    if (!gSPGlobal->getForwardManagerCore()->addForward(forwardPtr))
        return nullptr;

    return forwardPtr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(std::string_view name)
{
    auto result = m_plugins.find(name.data());

    return (result != m_plugins.end()) ? result->second : nullptr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(size_t index)
{
    for (const auto &entry : m_plugins)
    {
        if (entry.second->getId() == index)
            return entry.second;
    }
    return nullptr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(SourcePawn::IPluginContext *ctx)
{
    char *pluginIdentity;
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

    return getPluginCore(pluginIdentity);
}

IPlugin *PluginMngr::loadPlugin(const char *name,
                                char *error,
                                size_t size)
{
    std::string errorMsg;
    auto plugin = loadPluginCore(name, &errorMsg);

    if (!plugin)
    {
        std::strncpy(error, errorMsg.c_str(), size);
        error[size - 1] = '\0';
        return nullptr;
    }

    return plugin.get();
}

std::shared_ptr<Plugin> PluginMngr::loadPluginCore(std::string_view name,
                                                    std::string *error)
{
    auto plugin = _loadPlugin(gSPGlobal->getScriptsDirCore() / name.data(), error);
    if (!plugin)
        return nullptr;

    return plugin;
}

std::shared_ptr<Plugin> PluginMngr::_loadPlugin(const fs::path &path,
                                                std::string *error)
{
    auto pluginId = m_plugins.size();

    // Omit any unknown extension
    if (path.extension().string() != ".smx")
        return nullptr;

    auto fileName = path.stem().string();
    std::shared_ptr<Plugin> plugin;
    try
    {
        plugin = std::make_shared<Plugin>(pluginId, fileName, path);
    }
    catch (const std::exception &e)
    {
        *error = e.what();
        return nullptr;
    }

    if (const auto [iter, added] = m_plugins.try_emplace(fileName, plugin); !added)
        return nullptr;

    return plugin;
}

size_t PluginMngr::loadPlugins()
{
    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(gSPGlobal->getScriptsDirCore(), errCode);
    auto &loggingSystem = gSPGlobal->getLoggerCore();

    if (errCode)
    {
        loggingSystem->LogMessageCore("Error while loading plugins: ", errCode.message());
        return 0;
    }

    std::string errorMsg;
    for (const auto &entry : directoryIter)
    {
        auto filePath = entry.path();
        if (!_loadPlugin(filePath, &errorMsg) && !errorMsg.empty())
        {
            loggingSystem->LogErrorCore(errorMsg);
            errorMsg.clear();
        }
    }

    // After first binding let plugins add their natives
    gSPGlobal->getForwardManagerCore()->findForward("OnPluginNatives")->execFunc(nullptr);

    // Try to bind unbound natives
    const std::unique_ptr<NativeMngr> &nativeManager = gSPGlobal->getNativeManagerCore();
    for (const auto &entry : m_plugins)
    {
        SourcePawn::IPluginRuntime *runtime = entry.second->getRuntime();
        uint32_t nativesNum = runtime->GetNativesNum();
        for (uint32_t index = 0; index < nativesNum; ++index)
        {
            const sp_native_t *pluginNative = runtime->GetNative(index);

            if (pluginNative->status == SP_NATIVE_BOUND)
                continue;

            std::shared_ptr<Native> native = nativeManager->getNativeCore(pluginNative->name);
            if (!native)
                continue;

            runtime->UpdateNativeBinding(index, native->getRouter(), 0, nullptr);
        }
    }

    gSPGlobal->getForwardManagerCore()->findForward("OnPluginInit")->execFunc(nullptr);
    gSPGlobal->getForwardManagerCore()->findForward("OnPluginsLoaded")->execFunc(nullptr);
    return m_plugins.size();
}
