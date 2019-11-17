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

namespace SPSQLiteModule
{
    SQLiteStatement::SQLiteStatement(SQLiteHandler *handle, const char *sql) : m_handle(handle), m_sql(sql)
    {
        const int ret = sqlite3_prepare_v2(m_handle->getHandle(), m_sql, sizeof(m_sql), &m_stmt, NULL);
        if (ret != 0)
        {
        }
    }

    bool SQLiteStatement::hasResult(char *errormsg, std::size_t size)
    {
        const int ret = checkStep();

        if ((SQLITE_ROW != ret) && (SQLITE_DONE != ret))
        {
            gSPGlobal->getUtils()->strCopy(errormsg, size, sqlite3_errmsg(m_handle));
        }

        return m_hasRow;
    }

    ISQLiteColumn *SQLiteStatement::getResult()
    {
        try
        {
            if (m_hasDone)
            {
                m_currentIndex = 0;
                return NULL;
            }

            return m_columns.emplace_back(std::make_unique<SQLiteColumn>(m_stmt, m_currentIndex)).get()
        }
        catch (std::runtime_error &e)
        {
            return nullptr;
        }
    }

    void SQLiteStatement::bind(int index, const int value)
    {
        sqlite3_bind_int(m_stmt, index, value);
    }

    void SQLiteStatement::bind(int index, const float value)
    {
        sqlite3_bind_double(m_stmt, index, value);
    }

    void SQLiteStatement::bind(int index, const char *value)
    {
        sqlite3_bind_text(m_stmt, index, value, -1, SQLITE_TRANSIENT);
    }

    void SQLiteStatement::bind(int index, const void *value, std::size_t size)
    {
        sqlite3_bind_blob(m_stmt, index, value, size, SQLITE_TRANSIENT);
    }

    int SQLiteStatement::checkStep()
    {
        if (m_hasDone == false)
        {
            const int ret = sqlite3_step(m_stmt);
            if (SQLITE_ROW == ret)
            {
                m_hasRow = true;
            }
            else if (SQLITE_DONE == ret)
            {
                m_hasRow = false;
                m_hasDone = true;
            }
            else
            {
                m_hasRow = false;
                m_hasDone = false;
            }
            return ret;
        }
        else
        {
            return SQLITE_MISUSE;
        }
    }
} // namespace SPSQLiteModule