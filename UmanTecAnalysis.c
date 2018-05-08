/*
UmanTec Problem 2
Author: Caleb Birch
Description: The program analyses an input stream and displays the results.
The input gets taken from stdin, the output gets displayed on stdout, and error messages are written to stderr.
The user enters four packets of data which form a conversation. Once the whole conversation has been entered,
it is displayed. Multiple conversations can happen at the same time and each conversation is identified with a guid.
If the packet is invalid, it is discarded. The packet type starts with connect_0 or disconnect_0 and ends with
connect_3 or disconnect_3
*/
/*
A conversation consisting of four packets can be entered as a command line parameter. 
Each packet and each packet field should be separated by a single space.
Example: "5 65620330235 connect_0 0xd88039fffed09bc8 6 65620330583 connect_1 0xd88039fffed09bc8 7 65624012800 connect_2 0xd88039fffed09bc8 9 65624013554 connect_3 0xd88039fffed09bc8"
The argv[1] parameter is analysed if it is available.
After anlysing the argv[1] parameter, the program starts reading packets from stdin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACKET_SIZE 2048
#define MAX_GUID_SIZE 30
#define MAX_CONV_AMOUNT 20
#define MAX_PACKETTYPE_LEN 20
#define MAX_TIME_DIFF 1000000000
#define MAX_PACKET_STORAGE 80

int checkPacketErrors(int* packetID, char *token[], long long int pktNo[], long long int receiveTime[], char packetType[MAX_PACKET_STORAGE][MAX_PACKETTYPE_LEN], char guid[MAX_PACKET_STORAGE][MAX_GUID_SIZE]);

int main(int argc, char* argv[])
{
	char line[MAX_PACKET_SIZE];  				//Array to store the line entered for the packet
	char *testArray;							//Temporarily store the contents of line to check its validity with strchr
	char *eof = NULL;
	char *packet1;								//Holder to be used with strtok function
	long long int pktNo[MAX_PACKET_STORAGE];            		//Used to store each packet number
	long long int receiveTime[MAX_PACKET_STORAGE];     		 	//Used to store each receive time
	long long int receiveDiff;         			//Hold the difference between receive times
	char packetType[MAX_PACKET_STORAGE][MAX_PACKETTYPE_LEN];            		//Used to store each packet type
	char guid[MAX_PACKET_STORAGE][MAX_GUID_SIZE];        		//Used to store each guid
	char *token[20];                    		//Stores each token obtained with strtok()
	char conversation[MAX_CONV_AMOUNT][MAX_GUID_SIZE];	//Stores all conversation guids that were entered thus far
	int displayFirstConversation = 1;			
	int convSize = 1;							//Stores the current amount of conversations running
	int maxConvSize = 1;						//Stores the maximum amount of conversations reached.
	int addToArray = 0;							//Variable to indicate whether the guid should be added to new conversation	
	char delimiter = ' ';						//The delimiter separating the tokens in the input line
	int packetID;								//Stores the current packetID
	int convCount = 1;							//Keep track of the packet order to determine if the conversation is complete
	//Control loop counters
	int j = 1;									
	int x;								
	int i;
	int a; 

	line[sizeof(line)-1] = ~'\0';  /* Ensure no false-null at end of buffer */
	
	//If a parameter was entered when calling the program, analyse this parameter
	if(argc == 2)
	{
		strcpy(line, argv[1]);
	
		for (j = 1, packet1 = line; ; j++, packet1 = NULL) 
		{
		        token[j] = strtok(packet1, &delimiter);
		       // fprintf(stderr, "%s", token[j]);
		        if (token[j] == NULL)
		            break;
		        //printf("%d: %s\n", j, token);
		}
		
		//Continue only if enough tokens were obtained
		if(j == 17)
		{
			//Store the 1st packet
			pktNo[1] = atoi(token[1]);
		    receiveTime[1] = atoll(token[2]);
		    strcpy(packetType[1], token[3]);
		    strcpy(guid[1], token[4]);   
		
			if((strcmp("connect_0", packetType[1]) != 0) && (strcmp("disconnect_0", packetType[1]) != 0))
			{
				fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[1], receiveTime[1], packetType[1], guid[1]);
			}
			
			//Store the 2nd packet		
			pktNo[2] = atoi(token[5]);
		    receiveTime[2] = atoll(token[6]);
		    strcpy(packetType[2], token[7]);
		    strcpy(guid[2], token[8]); 
		    
		    //Store the 3rd packet
		    pktNo[3] = atoi(token[9]);
		    receiveTime[3] = atoll(token[10]);
		    strcpy(packetType[3], token[11]);
		    strcpy(guid[3], token[12]); 
		    
		    //Store the 4th packet
		    pktNo[4] = atoi(token[13]);
		    receiveTime[4] = atoll(token[14]);
		    strcpy(packetType[4], token[15]);
		    strcpy(guid[4], token[16]);
		    
		    //Compare the 4 packets to check that they conform to the spec
		    for(i = 1; i < 4; i++)
		    {
		    	if(receiveTime[i] >= receiveTime[i + 1])
				{
					fprintf(stderr, "{\"error\":{\"message\":\"Receive time less than or equal to previous packet\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[i + 1], receiveTime[i + 1], packetType[i + 1], guid[i + 1]);
					displayFirstConversation = 0;
					break;
				}
				else
				{
					receiveDiff = receiveTime[i + 1] - receiveTime[i];
					if(receiveDiff > MAX_TIME_DIFF)
					{
						fprintf(stderr, "{\"error\":{\"message\":\"Packet not received within 1s of previous packet\",\"packet\":");
						fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[i + 1], receiveTime[i + 1], packetType[i + 1], guid[i + 1]);
						displayFirstConversation = 0;
						break;
					}
				}
				
				if(pktNo[i] >= pktNo[i + 1])
				{
					fprintf(stderr, "{\"error\":{\"message\":\"invalid packet number\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[i + 1], receiveTime[i + 1], packetType[i + 1], guid[i + 1]);
					displayFirstConversation = 0;
					break;
				}
				
				if(strcmp(packetType[i], packetType[i + 1]) >= 0)
				{
					fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[i + 1], receiveTime[i + 1], packetType[i + 1], guid[i + 1]);
					displayFirstConversation = 0;
					break;
				}
				
				if(strcmp(guid[i], guid[i + 1]) != 0)
				{
					fprintf(stderr, "{\"error\":{\"message\":\"invalid guid\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[i + 1], receiveTime[i + 1], packetType[i + 1], guid[i + 1]);
					displayFirstConversation = 0;
					break;
				}
			} //end for
			
			//If there were no errors, display the conversation to stdout
			if(displayFirstConversation)
			{
				printf("\n{\"Completed Conversation\":[");
				x = 1; 				
				for(i = 1; i < 5; i++)
				{
	    			printf("{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}", pktNo[i], receiveTime[i], packetType[i], guid[i]);
	    			if(x < 4)
	    			{
	    				printf(",");
					}
	    			x++;
				}
				printf("]}\n");
			}	
		}	
	}
	else
	{
		fprintf(stderr, "{\"error\":{\"message\":\"No command line input parameter, will continue with stdin as input\"\n");
	}
		
	//Loop gets one packet at a time and cycles indefinitely until user enters "end"
	for(packetID = 1; ; packetID++)
	{
		//Gets the input line from stdin and checks if its valid
		while(eof == NULL)
		{
			j = 1;
			printf("Please enter packet %d\n", packetID);
			eof = fgets(line, sizeof(line), stdin);	
			
			testArray = &line[0];					//The input line is checked to see if the number of tokens are valid
			while((testArray = strchr(testArray, ' ')) != NULL) 
			{
			    j++;
			    testArray++;
			}
			
			if(j < 4)
			{
				eof = NULL;
				//End the program if "end" was entered as input
				if(strcmp("end\n" , line) == 0)
				return 0;
				
				fprintf(stderr, "Invalid input for packet\n");
			}
			
		}
		
		fflush(stdin);
		eof = NULL;
		
		//Divide the input string up into tokens
		for (j = 1, packet1 = line; ; j++, packet1 = NULL) 
		{
	        token[j] = strtok(packet1, &delimiter);
	        if (token[j] == NULL)
	            break;
	        //printf("%d: %s\n", j, token);
		}
		
		//Store the tokens in variables
	    pktNo[packetID] = atoi(token[1]);
	    receiveTime[packetID] = atoll(token[2]);
	    strcpy(packetType[packetID], token[3]);
	    
		token[4][strcspn(token[4], "\n")] = '\0';			//Remove the newline char from guid
	    strcpy(guid[packetID], token[4]);   
	    
	    if(packetID >= 2)
	    {
	    	//Check if the current guid is different from the previous guid that was entered.
	    	if(strcmp(guid[packetID-1], token[4]) != 0)
		    {
				//loop through the conversation list, and check if the guid is new
		   		for(a = 1; a < (maxConvSize + 1); a++)
		   		{
		   			if(!strcmp(conversation[a], token[4]))  
		   			{
		   				addToArray = 0;
		   				break;
					}
					
					if((a == maxConvSize))
					{
						addToArray = 1;
					}
					
				}
				
				//Add the current guid to a new conversation
				if(addToArray == 1)
				{
					//Increase the total number of conversations
					convSize++;
					maxConvSize++;
					strcpy(conversation[maxConvSize], token[4]); 
					printf("New conversation added.\n");
					addToArray = 0;
					
					//Check that the packet type is correct. If not, discard the conversation
					if((strcmp("connect_0", packetType[packetID]) != 0) && (strcmp("disconnect_0", packetType[packetID]) != 0))
					{
						fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type\",\"packet\":");
						fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[packetID], receiveTime[packetID], packetType[packetID], guid[packetID]);
						convSize--;
						packetID--;
						conversation[maxConvSize][1] = '\n';
					}
				}
			
			}
			
			convCount = 1;
			
			//Compare the current packet with all the previous packets of the same guid
			//Display error messages to stderr when invalid packet found
			convCount = checkPacketErrors(&packetID, token, pktNo, receiveTime, packetType, guid);
			
			//Check if this was the last packet for the conversation, and if the packet order was correct.
            //Display an error message if there's a problem.
			if(((strcmp("connect_3", packetType[packetID]) == 0) || (strcmp("disconnect_3", packetType[packetID]) == 0)) && (strcmp("blank", guid[packetID]) != 0))
			{
				if(convCount < 4)
				{
					fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type, conversation has been discarded\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[packetID], receiveTime[packetID], packetType[packetID], guid[packetID]);
					convSize--;
					//Remove the conversation and packets with current guid from program
					for(i = 1; i <= packetID; i++)
					{
						if(!strcmp(guid[i], token[4]))
			    		{
			    			strcpy(guid[i], "blank"); 
							//printf("%lld, %lld, %s, %s\n",pktNo[i] , receiveTime[i], packetType[i], guid[i]);
						}
					}
					packetID--;
					for(a = 1; a < (maxConvSize + 1); a++)
			   		{
			   			if(!strcmp(conversation[a], token[4]))  
			   			{
			   				conversation[a][1] = '\n';	   			
						    break;
						}
						
					}
				}
			}
		}
		else			//Discard the packet if it was invalid and display an error message
		{
			strcpy(conversation[maxConvSize], token[4]); 
			if((strcmp("connect_0", packetType[packetID]) != 0) && (strcmp("disconnect_0", packetType[packetID]) != 0))
			{
				fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[packetID], receiveTime[packetID], packetType[packetID], guid[packetID]);
				packetID--;		//Decrement packetID to restart at conversation 1	
			}
		}	
		
		//Display the whole conversation if it is ready.
		if((strcmp("connect_3", packetType[packetID]) == 0) && (convCount == 4))
		{		
			//Run through conversations until current conversation ID is found			
			for(a = 1; a < (maxConvSize + 1); a++)
	   		{
	   			//Find the conversation ID in index
	   			if(!strcmp(conversation[a], token[4]))  
	   			{
	   				conversation[a][1] = '\n';
	   				convSize--;
	   				//Display the whole conversation on stdout
					printf("\n{\"Completed Conversation\":[");	  
					int x = 1; 			
					for(i = 1; i <= packetID; i++)
					{
						if(!strcmp(guid[i], token[4]))
			    		{
			    			
			    			printf("{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}", pktNo[i], receiveTime[i], packetType[i], guid[i]);
			    			if(x < 4)
			    			{
			    				printf(",");
							}
			    			x++;
			    			strcpy(guid[i], "blank");
						}
					}
					printf("]}\n");	
				    break;
				}
				
			}
		}
		else if((strcmp("disconnect_3", packetType[packetID]) == 0) && (convCount == 4))
		{
			//Run through conversations until current conversation ID is found
			for(a = 1; a < (maxConvSize + 1); a++)
	   		{
	   			//Find the conversation ID in index
	   			if(!strcmp(conversation[a], token[4]))  
	   			{
	   				conversation[a][1] = '\n';
	   				convSize--;
	   				//Display the whole conversation on stdout
					printf("\n{\"Completed Conversation\":[");	  
					int x = 1; 	
							
					for(i = 1; i <= packetID; i++)
					{
						if(!strcmp(guid[i], token[4]))
			    		{
			    			printf("{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}", pktNo[i], receiveTime[i], packetType[i], guid[i]);
			    			if(x < 4)
			    			{
			    				printf(",");
							}
			    			x++;
			    			strcpy(guid[i], "blank");
						}
					}
					printf("]}\n");
				    break;
				}
			}
		}
		
		if(convSize < 1)
		{
			convSize = 1;
		}
		
	} //For
	
    return 0;
}

//Analyses the packet for errors in packet number, receive time, or packet type
int checkPacketErrors(int* packetID, char *token[], long long int pktNo[], long long int receiveTime[], char packetType[MAX_PACKET_STORAGE][MAX_PACKETTYPE_LEN], char guid[MAX_PACKET_STORAGE][MAX_GUID_SIZE])
{
	long long int receiveDiff;
	int i;
	int convCount = 1;
	
	for(i = 1;i < *packetID; i++)
	{
		if(!strcmp(guid[i], token[4]))
	    {
	    	//Display an error message if the receive time was invalid
			if(receiveTime[i] >= receiveTime[*packetID])
			{
				fprintf(stderr, "{\"error\":{\"message\":\"Receive time less than or equal to previous packet\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[*packetID], receiveTime[*packetID], packetType[*packetID], guid[*packetID]);
				*packetID = *packetID - 1;
				break;
			}
			else
			{
				receiveDiff = receiveTime[*packetID] - receiveTime[i];
				if(receiveDiff > MAX_TIME_DIFF)
				{
					fprintf(stderr, "{\"error\":{\"message\":\"Packet not received within 1s of previous packet\",\"packet\":");
					fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[*packetID], receiveTime[*packetID], packetType[*packetID], guid[*packetID]);
					*packetID = *packetID - 1;
					break;
				}
			}
			
			//Display an error message if the packet number was invalid
			if(pktNo[i] >= pktNo[*packetID])
			{
				fprintf(stderr, "{\"error\":{\"message\":\"invalid packet number\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[*packetID], receiveTime[*packetID], packetType[*packetID], guid[*packetID]);
				*packetID = *packetID - 1;
				break;
			}
			
			 //Display an error message if the packet type was invalid
			if(strcmp(packetType[i], packetType[*packetID]) > 0)
			{
				fprintf(stderr, "{\"error\":{\"message\":\"invalid packet type\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[*packetID], receiveTime[*packetID], packetType[*packetID], guid[*packetID]);
				*packetID = *packetID - 1;
				break;
			}
			else if(strcmp(packetType[i], packetType[*packetID]) == 0)
			{
				fprintf(stderr, "{\"error\":{\"message\":\"conversation with guid already exists\",\"packet\":");
				fprintf(stderr, "{\"packet_number\":\"%lld\",\"receive_time\":\"%lld\",\"packet_type\":\"%s\",\"guid\":\"%s\"}}}\n", pktNo[*packetID], receiveTime[*packetID], packetType[*packetID], guid[*packetID]);
				*packetID = *packetID - 1;
				break;
			}
			else
			{
				convCount++;

			}
				
		}
	}
	
	return convCount;
}
