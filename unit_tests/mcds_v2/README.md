Using M1Pro (Silicon chip) with macOS 12.4 and
```
$ g++-11 --version
g++-11 (Homebrew GCC 11.3.0_2) 11.3.0

$ file /opt/homebrew/bin/g++-11
/opt/homebrew/bin/g++-11: Mach-O 64-bit executable arm64
```

Using the incorrect method of writing the binary vector:
```
$ g++-11 -std=c++11 write_vec.cpp -o write_vec
$ ./write_vec
$ python read_vec.py 
5.21501721064226e-310 5.21501721064345e-310 5.21501721064345e-310
```

Using the correct method of writing the binary vector:
```
$ g++-11 -std=c++11 write_vec.cpp -o write_vec
$ ./write_vec
$ python read_vec.py 
42.1 42.2 42.3
```

