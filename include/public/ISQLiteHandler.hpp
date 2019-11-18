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

namespace SPMod
{
    class ISQLiteStatement;
    class ISQLiteHandler
    {
    public:
        /**
         * @brief Prepares and executes a query.
         *
         * @param sql The query string
         *
         * @return SQLiteStatement pointer
         */
        virtual ISQLiteStatement *query(const char *sql) = 0;

        /**
         * @brief
         *
         * @param sql
         * @return ISQLiteStatement*
         */
        virtual ISQLiteStatement *execute(const char *sql) = 0;

        /**
         * @brief Returns error code.
         * 
         * @param handle 
         * @return int 
         */
        virtual int errorCode(sqlite3* handle) = 0;

        /**
         * @brief Returns error message.
         * 
         * @param handle 
         * @return const char* 
         */
        virtual const char* errorInfo(sqlite3* handle) = 0;

    protected:
        virtual ~ISQLiteHandler() = default;
    };
} // namespace SPMod