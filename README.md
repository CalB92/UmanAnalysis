# UmanAnalysis
Author: Caleb Birch
Date: 8 May 2018

This program analyses a packet stream and reports the conversations found in the stream. When calling the program via the command line, a parameter can be added which will be analysed first before running the rest of the code. The following is an example of how the program can be called:

./Analysis "5 65620330235 connect_0 0xd88039fffed09bc8 6 65620330583 connect_1 0xd88039fffed09bc8 7 65624012800 connect_2 0xd88039fffed09bc8 9 65624013554 connect_3 0xd88039fffed09bc8"

All the fields must be separated by a single space. The keyboard was used as stdin when writing this code. The program will display the results on stdout if no errors were found in the packets. A conversation consists of four packets which were entered sequentially. An error message is displayed on stderr if one of the packets do not conform to the specification.

I'm not sure what the complex use case is that would break the program. I couldn't get it to crash with the inputs I used. I think it may have something to do with the packet type but the code checks the packet type to ensure it is entered in order.



