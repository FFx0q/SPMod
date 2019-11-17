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

#pragma once

#include "ext.hpp"

namespace SPSQLiteModule
{
    class SQLiteHandler : public SPMod::ISQLiteHandler
    {
    public:
        explicit SQLiteHandler(sqlite3 *handle);
        void setHandle(sqlite3 *handle);
        sqlite3 *getHandle();

        // ISQLiteHandler
        SPMod::ISQLiteStatement *query(const char *sql) override;
        SPMod::ISQLiteStatement *exec(const char *sql) override;

    private:
        sqlite3 *m_handle;
        std::vector<std::unique_ptr<SQLiteStatement>> m_stmts;
    };
} // namespace SPSQLiteModule