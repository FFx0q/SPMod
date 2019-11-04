/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

#include "ext.hpp"

ISqliteHandler* SPSqliteModule::SqliteInterface::connect(const char* filename)
{
    sqlite3* handle;
    auto handle_ptr = std::make_unique<SqliteHandler>(&handle);

    auto rc = sqlite3_open(filename, &handle);

    if (rc != SQLITE_OK)
    {
        //error
    }

    try {
        m_handles.push_back(handle_ptr);
        return handle_ptr.get();
    } catch(std::runtime_error &e) {
        return nullptr;
    }
}

bool SPSqliteModule::SqliteInterface::disconnect(ISqliteHandler* handle)
{
    auto rc = sqlite3_close(handle->getHandle());

    if (rc != SQLITE_OK)
        return false;
    return true;
}

bool SPSqliteModule::SqliteInterface::isValid(ISqliteHandler* handle)
{
    for (const auto& hndl : m_handles)
    {
        if (hndl.get() == handle)
            return true;   
    }
    return false;
}