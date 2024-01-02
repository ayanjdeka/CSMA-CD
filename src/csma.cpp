#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef struct{
    int id; 
    int level_left;
    int left;
} node_t;



typedef struct{
    int N;
    int L;
    int M; 
    int transaction_state;
    unsigned long T; 
    unsigned long current_state; 
    unsigned long successful_transaction;
    vector<int> R;
    vector<node_t> hosts;
} csma_struct;

static csma_struct csma;

int main(int argc, char** argv) {

    if (argc != 2) {

        printf("Usage: ./csma input.txt\n");

        return -1;

    }

    FILE* outfile = fopen("output.txt", "w");

    string file = argv[1];

    csma.current_state = 0;

    csma.successful_transaction = 0;

    csma.transaction_state = 0;

    ifstream infile;

    infile.open(file);

    string line;

    while (getline(infile, line)) {

        char* str = const_cast<char *>(line.c_str());

        long val;

        if (*str == 'N') { 

    		sscanf(str + 1, "%d", &csma.N);

    	} else if (*str == 'L') { 

    		sscanf(str + 1, "%d", &csma.L);

    	} else if (*str == 'M') { 

    		sscanf(str + 1, "%d", &csma.M);

    	} else if (*str == 'T') { 

    		sscanf(str + 1, "%lu", &csma.T);

    	} else if (*str == 'R') {

            val = strtol(str + 1, &str, 10);

            while (val) {

            	csma.R.push_back(val); 

            	val = strtol(str, &str, 10);

            }

        }

    }



    for (int i = 0; i < csma.N; i++) {

        node_t host;

        host.level_left = 0;

        host.id = i;

        host.left = host.id % csma.R[host.level_left];

        csma.hosts.push_back(host);

    }



    infile.close();

    

    while(csma.current_state != csma.T - 1) {

    	    vector<int> temp_vec;

	    for (int i = 0; i < csma.N; i++) { 

	    	if (csma.hosts[i].left == 0) { 

	    		temp_vec.push_back(i);

	    	}

	    }

	    if (temp_vec.size() > 1) {

		for (int i = 0; i < temp_vec.size(); i++) {

		    if (++csma.hosts[temp_vec[i]].level_left == csma.M) { 

		    	csma.hosts[temp_vec[i]].level_left = 0;

		    }

		    csma.hosts[temp_vec[i]].left = (csma.hosts[temp_vec[i]].id + csma.current_state + 1) % csma.R[csma.hosts[temp_vec[i]].level_left];

		}

	    } else if (temp_vec.size() == 1) {

		csma.hosts[temp_vec[0]].level_left = 0;

		csma.hosts[temp_vec[0]].left = (csma.hosts[temp_vec[0]].id + csma.current_state + csma.L) % csma.R[csma.hosts[temp_vec[0]].level_left];

		if (csma.current_state + csma.L < csma.T) {

			csma.current_state += csma.L - 1; 

			csma.successful_transaction += csma.L;

		} else {

			csma.successful_transaction += csma.T - csma.current_state; 	

			csma.current_state = csma.T - 1;

		}

	    } else for (int i = 0; i < csma.N; i++) csma.hosts[i].left--;

	    csma.current_state++;

    }

    fprintf(outfile, "%.2lf\n", (double) csma.successful_transaction / (double) csma.T);

    fclose(outfile);

    return 0;

}
