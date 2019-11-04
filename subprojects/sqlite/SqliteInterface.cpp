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

namespace SPSqliteModule 
{
    const std::unique_ptr <SqliteHandler> &SqliteInterface::getFreeHandle()
    {
        for(const auto& handle : m_handles)
        {
            if (handle->getHandle()) continue;
            return handle;
        }
        static std::unique_ptr<SqliteHandler> dummy;
        return dummy;
    }

    SPMod::ISqliteHandler* SqliteInterface::connect(const char* filename, char* errormsg, std::size_t size)
    {
        try {
            sqlite3* handle;
            int rc = sqlite3_open(filename, &handle);

            if (rc != SQLITE_OK)
            {
                gSPGlobal->getUtils()->strCopy(errormsg, size, sqlite3_errmsg(handle));
                return nullptr;
            }
            
            if (const std::unique_ptr<SqliteHandler> &handle_ptr = getFreeHandle(); handle_ptr) {
                handle_ptr->setHandle(handle);
                return handle_ptr.get();
            } else {
                return m_handles.emplace_back(std::make_unique<SqliteHandler>(handle)).get();
            }
        } catch(std::runtime_error &e) {
            return nullptr;
        }
    }

    bool SqliteInterface::disconnect(SPMod::ISqliteHandler* handle)
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

    bool SqliteInterface::isValid(SPMod::ISqliteHandler* handle)
    {
        auto it = std::find_if(m_handles.begin(), m_handles.end(),
        [&](const std::unique_ptr<SqliteHandler> &p) {
            return p.get() == handle;
        });

        if (!(*it)->getHandle()) 
            return false;
    
        return true;
    }
}