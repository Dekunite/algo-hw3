/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <19/05/2021> 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>

#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;

double penalty = -2;
int ind;

double similarityScore(char a, char b)
{
	double result;
	if(a==b)
	{
		result=1;
	}
	else
	{
		result=penalty;
	}
	return result;
}

double findMax(double array[], int length)
{
	double max = array[0];
	ind = 0;

	for(int i=1; i<length; i++)
	{
		if(array[i] > max)
		{
			max = array[i];
			ind=i;
		}
	}
	return max;
}



int main() {

  string fname;
  fname = "strings1.txt";
	//get file name
  //cin >> fname;
  ifstream filename(fname);

  string word;
	vector<string> words;

	//read file
  while (filename >> word) {
		words.push_back(word);
	}

	//sort words alphabetically
	sort(words.begin(), words.end());

	/*
	//print words
	for (int i=0; i<words.size(); i++) {
		cout << words.at(i) << endl;
	}
	*/

	/*
	string word1 = words[1];
	string word2 = words[2];
	cout << "word1: " << word1 << " word2: " << word2 << endl;
	*/

	string word1, word2;
	int firstWordCounter = 0;
	int secondWordCounter = 0;
	int secondWordRef = 0;
	int tick;
	//vector to hold overlap letters
	vector<string> overlaps;

	//loop through words
	for (firstWordCounter; firstWordCounter<words.size(); firstWordCounter++) {
		//skip over comparing the same words
		if(firstWordCounter == secondWordCounter) {
			secondWordCounter++;
		}
		for (secondWordCounter; secondWordCounter < words.size(); secondWordCounter++) {

			word1 = words[firstWordCounter];
			word2 = words[secondWordCounter];

			//word lengths
			int length1 = word1.length();
			int length2 = word2.length();

			char consensus_a[length1 + length2 + 2];
			char consensus_b[length1 + length2 + 2];

			//init matrix
			double matrix[length1+1][length2+1];
			for(int i = 0; i<=length1; i++) {
				for(int j=0; j<=length2; j++) {
					//her yeri 0 la
					matrix[i][j]=0;
				}
			}

			double traceback[4];
			int I_i[length1+1][length2+1];
			int I_j[length1+1][length2+1];

			//filling the matrix
			for(int i=1; i<=length1; i++) {
				for(int j = 1; j<=length2; j++) {
					//cout << i << " " << j << endl;
					traceback[0] = matrix[i-1][j-1]+similarityScore(word1[i-1], word2[j-1]);
					traceback[1] = matrix[i-1][j]+penalty;
					traceback[2] = matrix[i][j-1]+penalty;
					traceback[3] = 0;
					matrix[i][j] = findMax(traceback, 4);

					switch(ind)
					{
						case 0:
							I_i[i][j] = i-1;
							I_j[i][j] = j-1;
							break;
						case 1:
							I_i[i][j] = i-1;
							I_j[i][j] = j;
							break;
						case 2:
							I_i[i][j] = i;
							I_j[i][j] = j-1;
							break;
						case 3:
							I_i[i][j] = i;
							I_j[i][j] = j;
							break;
					}
				}
			}

			/*
			// print the scoring matrix to console
			for(int i=0;i<=length1;i++)
			{
				for(int j=0;j<=length2;j++)
				{
					cout << matrix[i][j] << " ";
				}
				cout << endl;
			}
			*/

			//find max score in matrix
			double matrix_max = 0;
			int i_max = 0, j_max = 0;
			int maxScoreCount = 0;
			//vector to hold index of maximum numbers (i, j)
			vector<int> iAndJMax;
			for(int i = 1; i <= length1; i++) {
				for (int j = 1; j <= length2; j++) {

					if(matrix[i][j] > matrix_max) {
						//reset score count and vector if maximimum increases
						maxScoreCount = 0;
						iAndJMax.clear();

						matrix_max = matrix[i][j];
						i_max = i;
						j_max = j;
						
						iAndJMax.push_back(i_max);
						iAndJMax.push_back(j_max);

						//record multiple maximums
					} else if (matrix[i][j] == matrix_max) {
						i_max = i;
						j_max = j;
						
						iAndJMax.push_back(i_max);
						iAndJMax.push_back(j_max);
						maxScoreCount++;
					}

				}
			}

			//cout << "Max score in the matrix is " << matrix_max << endl;

			for (int k = 0; k < iAndJMax.size(); k += 2) {
				//assign the pair of max i and j
				i_max = iAndJMax[k];
				j_max = iAndJMax[k+1];

				//traceback
				int current_i = i_max;
				int current_j = j_max;
				int next_i = I_i[current_i][current_j];
				int next_j = I_j[current_i][current_j];
				tick = 0;

				while(((current_i != next_i) || (current_j != next_j)) && (next_i >= 0) && (next_i >= 0)) {

					//cout << word1[current_i-1] << endl;

					if(next_i == current_i) {
						//deletion in A
						consensus_a[tick] = '-';
					} else {
						//match/mismatch in A
						consensus_a[tick] = word1[current_i-1];
					}

					if(next_j == current_j) {
						//deletion in B
						consensus_b[tick] = '-';
					} else {
						//match/mismatch in B
						consensus_b[tick] = word2[current_j-1];
					}

					if (next_i == 0) {
						next_i = -1;
					} else if (next_j == 0) {
						next_j = -1;
					} else {
						current_i = next_i;
						current_j = next_j;
						next_i = I_i[current_i][current_j];
						next_j = I_j[current_i][current_j];
					}

					//push overlaps to the vector

					tick++;
				}
				overlaps.push_back(consensus_a);
				//overlaps.push_back(*consensus_b);

				//print consensus sequence
				//cout << "consesus a :" << consensus_a <<endl;
				//cout << "consesus b :" << consensus_b <<endl;
				//print output
				/*
					cout << word1 << " - " << word2 << endl;
					cout << "Score: " << tick << " Sequence(s): " ;
					cout << "\"" ;
				for (int i = 0; i < overlaps.size(); i++) {
					*consensus_a = overlaps[i];
					for(int i = tick-1; i>=0; i--) {
						cout<<consensus_a[i]; 
					} 
				}
					cout << "\"" << endl;
					*/

				/*
				//ptritn conssss
				cout << consensus_a <<endl;
				cout << consensus_b <<endl;

				for(int i = tick-1; i>=0; i--) {
					cout<<consensus_a[i]; 
				}
				cout<<endl;
				for(int j = tick-1; j>=0; j--) {
					cout<<consensus_b[j];
				}
				cout<<endl;
				*/
			}

			//sort overlaps alphabetically
			sort(overlaps.begin(), overlaps.end());

			cout << word1 << " - " << word2 << endl;
			cout << "Score: " << tick << " Sequence(s): " ;
			for (int i = 0; i < overlaps.size(); i++) {
				string overlapLetters = overlaps[i];
				cout << "\"" ;
				for(int k = tick-1; k>=0; k--) {
					cout<<overlapLetters[k]; 
				} 
				cout << "\" ";
			}
			cout << endl;
			overlaps.clear();

		}

		secondWordRef++;
		secondWordCounter = secondWordRef;

	}

	cout <<"the end";
	
	return 0;
}