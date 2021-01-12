# MTA PostgreSQL driver	

# Decription	
Just a pgsql module for mta.	
I accept any help with developing the module. Feel free to make PRs and Issues.	

# Installation	
Read more info in [Releases](https://github.com/Disinterpreter/ml_pgsql/releases).

# Functions	
## userdata pg_conn(string connection)	
Make a connect with your postgres server.	
String can be like:	
`postgresql://USER:PASSWORD@IP:PORT/dbname?connect_timeout=3`	
or	
`hostaddr=IP port=5432 dbname=DBNAME user=USERNAME password=PASSWORD`	

**return** userdata with connect in sucsessful connection. or bool and string with error otherwise.	
```lua	
local conn,err = pg_conn("postgresql://user:123qwe@127.0.03:5432/mydb?connect_timeout=3");	
if (err ~= nil) then	
   print(err)	
   return 1;	
end	
```	
## userdata pg_query(userdata connection, string query)	
Send a data to the query string. String escaping is supporting.	

**return** userdata with a query. Otherwise bool and string error.	
```lua	
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)	
if (query == false)  then	
    iprint(qerr)	
end	
```	
## table pg_poll(userdata query)	
Get the data from the query userdata to the array	

**return** a table or TODO	
```lua	
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)	
if (query == false)  then	
    iprint(qerr)	
end	
iprint(pg_poll(query));	
```	
## bool pg_free(userdata query)	
Free a memory after executing query (if it don't happens)	

**return** true	
```lua	
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)	
pg_free(query)	
```	
## bool pg_exec(userdata connection, string query)	
Like a pg_query it make a query, but do not return any data.	

**return** bool as a result of executing the query. Otherwise bool and string error.	
```lua	
local exec = pg_exec(conn, "INSERT INTO users (name, password, money) VALUES ($1,$2,$3)", "a man", "mypasswd", "13");	
iprint(exec);	
```	
## bool pg_close(userdata connection);	
Close a database connection.	

**return** boolean	
```lua	
local conn,err = pg_conn("postgresql://user:123qwe@127.0.03:5432/mydb?connect_timeout=3");	
...	
pg_close(conn);	
```