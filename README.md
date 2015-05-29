# lightweight_logger
LightWeight and multithreaded logger in CPP11

This logger proposes 6 levels + off for logs.
Output in file or standard

#Example usage
```cpp
lightweight_logger *l = new lightweight_logger(lightweight_logger::lwl_lvl::LWL_TRACE);
*l << lightweight_logger::lwl_lvl::LWL_ERROR << "something to log" << std::endl;
delete l;
```

#Logs Levels

Prefix lightweight_logger::lwl_lvl::  
Example ```lightweight_logger::lwl_lvl::LWL_TRACE```

Level   |  Name       | value
--------|-------------|-------
Trace   | LWL_TRACE   |  0
Debug   | LWL_DEBUG   |  1
Info    | LWL_INFO    |  2
Warning | LWL_WARNING |  3
Error   | LWL_ERROR   |  4
Verbose | LWL_VERBOSE |  5
Off     | LWL_OFF     |  6

#Logging
######Standard ouput
```cpp
lightweight_logger *l = new lightweight_logger(lightweight_logger::lwl_lvl::LWL_TRACE);
```

######File
The file is named `named out.txt`
```cpp
lightweight_logger *l = new lightweight_logger(lightweight_logger::lwl_lvl::LWL_TRACE, "out.txt");
```
######Important
`std::endl` is used to notify the logger that he can write the message on the chosen output
