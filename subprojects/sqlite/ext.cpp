/*
 *  Copyright (C) 2019 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ext.hpp"

SPMod::ISPGlobal *gSPGlobal;
SPMod::ILogger *gLogger;
std::unique_ptr<SPSqliteModule::SqliteModuleInterface> gInterface;

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;

    if (!gSPGlobal->getLoggerManager()->isVersionCompatible(SPMod::ILoggerMngr::VERSION))
    {
        return SPMod::ExtQueryValue::DontLoad;
    }
    gInterface = std::make_unique<SPSqliteModule::SqliteModuleInterface>();

    gSPGlobal->registerModule(gInterface.get());
    return SPMod::ExtQueryValue::SPModModule;
}

SPMOD_API bool SPMod_Init()
{
    gLogger = gSPGlobal->getLoggerManager()->getLogger("SQLite");
#if defined NDEBUG
    gLogger->setLogLevel(SPMod::LogLevel::Info);
#else
    gLogger->setLogLevel(SPMod::LogLevel::Debug);
#endif


    gLogger->logToConsole(SPMod::LogLevel::Info, "Version: %s", SQLITE_VERSION);

    return true;
}

SPMOD_API void SPMod_End()
{
    return;
}