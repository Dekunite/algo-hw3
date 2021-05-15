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

double penalty = -10;
int index;

double checkSimilarity(char a, char b)
{
	double result;
	if(a==b) {
		result=1;
	}
	else {
		result=penalty;
	}

	return result;
}

double findMaximum(double traceback[], int length)
{
	double max = traceback[0];
	index = 0;

	for(int i=1; i<length; i++) {
		if(traceback[i] > max) {
			max = traceback[i];
			index=i;
		}
	}
	return max;
}



int main() {

  string fname;
  fname = "strings.txt";
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
	int counter;
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

			char commonSeq[length1 + length2 + 2];
			char consensus_b[length1 + length2 + 2];

			//init matrix
			double matrix[length1+1][length2+1];
			for(int i = 0; i <= length1; i++) {
				for(int j = 0; j <= length2; j++) {
					//her yeri 0 la
					matrix[i][j]=0;
				}
			}

			double traceback[4];
			int matrixI[length1+1][length2+1];
			int matrixJ[length1+1][length2+1];

			//filling the matrix
			for(int i = 1; i <= length1; i++) {
				for(int j = 1; j <= length2; j++) {
					//cout << i << " " << j << endl;
					traceback[0] = matrix[i-1][j-1] + checkSimilarity(word1[i-1], word2[j-1]);
					traceback[1] = matrix[i-1][j] + penalty;
					traceback[2] = matrix[i][j-1] + penalty;
					traceback[3] = 0;
					matrix[i][j] = findMaximum(traceback, 4);

					switch(index)
					{
						case 0:
							matrixI[i][j] = i-1;
							matrixJ[i][j] = j-1;
							break;
						case 1:
							matrixI[i][j] = i-1;
							matrixJ[i][j] = j;
							break;
						case 2:
							matrixI[i][j] = i;
							matrixJ[i][j] = j-1;
							break;
						case 3:
							matrixI[i][j] = i;
							matrixJ[i][j] = j;
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
			double maximum = 0;
			int iMax = 0, jMax = 0;
			int maxScoreCount = 0;
			//vector to hold index of maximum numbers (i, j)
			vector<int> iAndJMax;
			for(int i = 1; i <= length1; i++) {
				for (int j = 1; j <= length2; j++) {

					if(matrix[i][j] > maximum) {
						//reset score count and vector if maximimum increases
						maxScoreCount = 0;
						iAndJMax.clear();

						maximum = matrix[i][j];
						iMax = i;
						jMax = j;
						
						iAndJMax.push_back(iMax);
						iAndJMax.push_back(jMax);

						//record multiple maximums
					} else if (matrix[i][j] == maximum) {
						iMax = i;
						jMax = j;
						
						iAndJMax.push_back(iMax);
						iAndJMax.push_back(jMax);
						maxScoreCount++;
					}

				}
			}

			//cout << "Max score in the matrix is " << maximum << endl;

			for (int k = 0; k < iAndJMax.size(); k += 2) {
				//assign the pair of max i and j
				iMax = iAndJMax[k];
				jMax = iAndJMax[k+1];

				//traceback
				int currentI = iMax;
				int currentJ = jMax;
				int nextI = matrixI[currentI][currentJ];
				int nextJ = matrixJ[currentI][currentJ];
				counter = 0;

				while(((currentI != nextI) || (currentJ != nextJ)) && (nextI >= 0) && (nextJ >= 0)) {

					//cout << word1[currentI-1] << endl;

					if(nextI == currentI) {
						//deletion in A
						commonSeq[counter] = '-';
					} else {
						//match/mismatch in A
						commonSeq[counter] = word1[currentI-1];
					}

					if(nextJ == currentJ) {
						//deletion in B
						consensus_b[counter] = '-';
					} else {
						//match/mismatch in B
						//burada patlıyor
						consensus_b[counter] = word2[currentJ-1];
					}

					if (nextI == 0) {
						nextI = -1;
					} else if (nextJ == 0) {
						nextJ = -1;
					} else {
						currentI = nextI;
						currentJ = nextJ;
						nextI = matrixI[currentI][currentJ];
						nextJ = matrixJ[currentI][currentJ];
					}

					//push overlaps to the vector

					counter++;
				}
				string temp = commonSeq;
				if (!(find(overlaps.begin(), overlaps.end(), temp) != overlaps.end())) {
					overlaps.push_back(commonSeq);
				}
				//overlaps.push_back(*consensus_b);

				//print consensus sequence
				//cout << "consesus a :" << commonSeq <<endl;
				//cout << "consesus b :" << consensus_b <<endl;
				//print output
				/*
					cout << word1 << " - " << word2 << endl;
					cout << "Score: " << counter << " Sequence(s): " ;
					cout << "\"" ;
				for (int i = 0; i < overlaps.size(); i++) {
					*commonSeq = overlaps[i];
					for(int i = counter-1; i>=0; i--) {
						cout<<commonSeq[i]; 
					} 
				}
					cout << "\"" << endl;
					*/

				/*
				//ptritn conssss
				cout << commonSeq <<endl;
				cout << consensus_b <<endl;

				for(int i = counter-1; i>=0; i--) {
					cout<<commonSeq[i]; 
				}
				cout<<endl;
				for(int j = counter-1; j>=0; j--) {
					cout<<consensus_b[j];
				}
				cout<<endl;
				*/
			}

			//sort overlaps alphabetically
			sort(overlaps.begin(), overlaps.end());
			vector<string> sortedLetters;
			string newLetter = "";

			cout << word1 << " - " << word2 << endl;
			cout << "Score: " << counter << " Sequence(s): " ;
			for (int i = 0; i < overlaps.size(); i++) {
				string overlapLetters = overlaps[i];
				int newLetterCounter = 0;
				
				for(int k = counter-1; k>=0; k--) {
					//newLetter[newLetterCounter] = overlapLetters[k]; 
					newLetter += overlapLetters[k];
					newLetterCounter++;
				}
				string temp = newLetter;
				sortedLetters.push_back(temp);
				newLetter = "";
				//newLetter = ' ';
			}

			//sort
			sort(sortedLetters.begin(),sortedLetters.end());
			for(int i = 0; i < sortedLetters.size(); i++) {
				cout << "\"" ;
				cout << sortedLetters[i];
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