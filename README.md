# Postgre.SQL module for MTA-SA

# Description	
Simple module for MTA that provides Postgre.SQL client API.<br>	
I accept any support regarding the module development. Feel free to make PRs and Issues.	

# Installation	
Information about **Installation** provided on the **[Releases](https://github.com/Disinterpreter/ml_pgsql/releases)** page.

# Build from source
Check our [builds](BUILD.md) file.

# Functions	
## pg_conn function
### Syntax
```lua
userdata pg_conn(string connection)
```
### Description
Attempts to make connection with your Postgre.SQL server.

### Parameters
`string connection` - connection query string.<br>
Might look like:
* `postgresql://USER:PASSWORD@IP:PORT/dbname?connect_timeout=3`
* `hostaddr=IP port=5432 dbname=DBNAME user=USERNAME password=PASSWORD`

### Return value
If the connection attempt is successfull, the function returns **connection handle**.<br>
On the failure, the function returns 2 values:
* `false` (bool)
* `Error message` (string)

### Example of usage
```lua
local conn,err = pg_conn("postgresql://user:123qwe@127.0.03:5432/mydb?connect_timeout=3");	
if (err ~= nil) then
   print(err)
   return 1;
end
```

## pg_query function
### Syntax
```lua
userdata pg_query(userdata connection, string query)
```
### Description
Sends query to the Postgre.SQL server with provided data. String escaping supports as well.

### Parameters
`userdata connection` - connection handle (see pg_conn).<br>
`string query` - query to send to the Postgre.SQL server.

### Return value
If query was sent successfully, the function returns **userdata with the query result handle**.<br>
On the failure, the function returns 2 values:
* `false` (bool)
* `Error message` (string)

### Example of usage
```lua
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)
if (query == false)  then
    iprint(qerr)
end	
```
## pg_poll function
### Syntax
```lua
table pg_poll(userdata query)	
```
### Description
Gets the data from the query result handle.

### Parameters
`userdata query` - query result handle (see pg_query).

### Return value
On the success, the function returns a table.<br>
On the failure, the function returns 1 value:
* `false` (bool)

### Example of usage
```lua
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)
if (query == false)  then
    iprint(qerr)
end
iprint(pg_poll(query));
```

## pg_free function
### Syntax
```lua
bool pg_free(userdata query)
```

### Description
Frees memory after executing the query (in case it didn't happen before).

### Parameters
`userdata query` - query result handle (see pg_query).

### Return value
If the query result handle is valid, the function returns `true` (bool).<br>
Otherwise the function returns `false` (bool).

### Example of usage
```lua
local query,qerr = pg_query(conn, "SELECT $1 as a, $2 as b", -67, 2)
pg_free(query)
```

## pg_exec function
### Syntax
```lua
bool pg_exec(userdata connection, string query)
```

### Description
Sends query to the Postgre.SQL server, except it doesn't return any data like pg_query.

### Parameters
`userdata connection` - connection handle (see pg_conn).<br>
`string query` - query to send to the Postgre.SQL server.

### Return value
If query was successfully sent, the function returns userdata with the query result handle.<br>
On the failure, the function returns 2 values:
* `false` (bool)
* `Error message` (string)

### Example of usage
```lua
local exec = pg_exec(conn, "INSERT INTO users (name, password, money) VALUES ($1,$2,$3)", "a man", "mypasswd", "13");
iprint(exec);
```

## pg_close function
### Syntax
```lua
bool pg_close(userdata connection)
```

### Description
Closes connection with the Posgre.SQL server.

### Parameters
`userdata connection` - connection handle (see pg_conn).

### Return value
If connection was closed successfully, the function returns `true` (bool).<br>
On the failure, the function returns `false` (bool).

### Example of usage
```lua
local conn,err = pg_conn("postgresql://user:123qwe@127.0.03:5432/mydb?connect_timeout=3");

... (some code here)

pg_close(conn);
```
