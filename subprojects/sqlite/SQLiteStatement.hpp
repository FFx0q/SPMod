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
    class SQLiteStatement : public SPMod::ISQLiteStatement
    {
        SQLiteStatement(SQLiteHandler *handle, const char *sql);
        int checkStep();

        // ISQLiteStatement
        int affectedRows() override;
        bool hasResult(char *errormsg, std::size_t size) override;
        ISQLiteColumn *getResult() override;

        void bind(int index, const int value) override;
        void bind(int index, const float value) override;
        void bind(int index, const char *value) override;
        void bind(int index, const void *value, std::size_t size) override;

    private:
        std::vector<std::unique_ptr<SQLiteColumn>> m_columns;
        int m_currentIndex;
        sqlite3_stmt *m_stmt;
        const char *m_sql;
        SQLiteHandler *m_handle;
        bool m_hasRow;
        bool m_hasDone;
    };
} // namespace SPSQLiteModule