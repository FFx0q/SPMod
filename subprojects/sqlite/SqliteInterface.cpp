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

SPMod::ISqliteHandler* SPSqliteModule::SqliteInterface::connect(const char* filename)
{
    try {
        sqlite3* handle;
        int rc = sqlite3_open(filename, &handle);

        if (rc != SQLITE_OK)
        {
            //error
        }

        auto &handle = m_handles.emplace_back(std::make_unique<SqliteHandler>(handle));
        return handle.get();
    } catch(std::runtime_error &e) {
        return nullptr;
    }
}

bool SPSqliteModule::SqliteInterface::disconnect(SPMod::ISqliteHandler* handle)
{
    auto it = std::find_if(m_handles.begin(), m_handles.end(), 
    [&](const std::unique_ptr<SqliteHandler> &p) {
        return p.get() == handle;
    });

    int rc = sqlite3_close((*it)->getHandle());

    if (rc != SQLITE_OK)
        return false;
    return true;
}

bool SPSqliteModule::SqliteInterface::isValid(SPMod::ISqliteHandler* handle)
{
    (void)handle;
    return true;
}