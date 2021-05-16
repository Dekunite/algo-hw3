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

double gap = -4;
double mismatchValue = -2;
double match = 1;
int mismatchCount = 0;
int index;

double checkSimilarity(char a, char b) {
	double result;

	if(a == b) {
		result = match;
	}
	else {
		result = mismatchValue;
	}

	return result;
}

double findMaximum(double traceback[]) {
	double max = traceback[0];
	index = 0;

	for(int i = 1; i < 4; i++) {
		if(traceback[i] > max) {
			max = traceback[i];
			index=i;
		}
	}
	return max;
}



int main(int argc, char* argv[]) {

  string fname;
  string outputFname;
  fname = "strings.txt";
	outputFname = "output.txt";
	if (argc > 1) {
		fname = argv[1];
		cout << "fname: " << fname <<endl;;
	}
	if (argc > 2) {
		outputFname = argv[2];
		cout << "outputFname: " << outputFname <<endl;;
	}
	if (argc > 3) {
		match = atoi(argv[3]);
		cout << "match: " << match <<endl;;
	}
	if (argc > 4) {
		mismatchValue = atoi(argv[4]);
		cout << "mismatchValue: " << mismatchValue <<endl;;
	}
	if (argc > 5) {
		gap = atoi(argv[5]);
		cout << "gap: " << gap <<endl;;
	}
	/*
	*/
  ifstream filename(fname);

  string word;
	vector<string> words;

	//clear outputFname
	ofstream output;
	output.open(outputFname);
	output.close();

	//read file
  while (filename >> word) {
		words.push_back(word);
	}

	//sort words alphabetically
	sort(words.begin(), words.end());

	/*
	//print words
	for (int i=0; i< (int)words.size(); i++) {
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
	int counterScore;
	//vector to hold overlap letters
	vector<string> overlaps;

	//loop through words
	for (;firstWordCounter < (int)words.size(); firstWordCounter++) {
		//skip over comparing the same words
		if(firstWordCounter == secondWordCounter) {
			secondWordCounter++;
		}
		for (; secondWordCounter < (int)words.size(); secondWordCounter++) {

			word1 = words[firstWordCounter];
			word2 = words[secondWordCounter];

			//word lengths
			int length1 = word1.length();
			int length2 = word2.length();

			char commonSeq1[length1 + length2 + 2];
			//char commonSeq2[length1 + length2 + 2];

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
					traceback[1] = matrix[i-1][j] + gap;
					traceback[2] = matrix[i][j-1] + gap;
					traceback[3] = 0;
					matrix[i][j] = findMaximum(traceback);

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
			//print matrix
			for(int i=0;i<=length1;i++)
			{
				for(int j=0;j<=length2;j++)
				{
					cout << matrix[i][j] << " ";
				}
				cout << endl;
			}
			*/

			double maximum = 0;
			int maxI = 0, maxJ = 0;
			//vector to hold index of maximum numbers (i, j)
			vector<int> iAndJMax;
			//find maximum number in the matrix
			for(int i = 1; i <= length1; i++) {
				for (int j = 1; j <= length2; j++) {

					if(matrix[i][j] > maximum) {
						//reset vector if maximimum increases
						iAndJMax.clear();

						maximum = matrix[i][j];
						maxI = i;
						maxJ = j;
						
						iAndJMax.push_back(maxI);
						iAndJMax.push_back(maxJ);

						//record multiple maximums
					} else if (matrix[i][j] == maximum) {
						maxI = i;
						maxJ = j;
						
						iAndJMax.push_back(maxI);
						iAndJMax.push_back(maxJ);
					}

				}
			}

			//cout << "Max score in the matrix is " << maximum << endl;

			for (int k = 0; k < (int)iAndJMax.size(); k += 2) {
				//assign the pair of max i and j
				maxI = iAndJMax[k];
				maxJ = iAndJMax[k+1];

				//traceback
				int currentI = maxI;
				int currentJ = maxJ;
				int nextI = matrixI[currentI][currentJ];
				int nextJ = matrixJ[currentI][currentJ];
				counter = 0;
				counterScore = 0;

				while(((currentI != nextI) || (currentJ != nextJ)) && (nextI >= 0) && (nextJ >= 0)) {

					//cout << word1[currentI-1] << endl;

					if(nextI == currentI) {
						//deletion in A
						commonSeq1[counter] = '-';
					} else {
						//match/mismatch in A
						commonSeq1[counter] = word1[currentI-1];
						counter++;
						counterScore += match;
					}

					if(nextJ == currentJ) {
						//deletion in B
						//commonSeq2[counter] = '-';
					} else {
						//match/mismatch in B
						//burada patlıyor
						//commonSeq2[counter] = word2[currentJ-1];
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
					/*
					counter+=match;
					counterDivMatch = counter/match;
					*/
				}
				string temp = commonSeq1;
				//skip over the same common sequences, dont push them to vector
				if (!(find(overlaps.begin(), overlaps.end(), temp) != overlaps.end())) {
					overlaps.push_back(commonSeq1);
				}
				//overlaps.push_back(*commonSeq2);

				//print consensus sequence
				//cout << "consesus a :" << commonSeq1 <<endl;
				//cout << "consesus b :" << commonSeq2 <<endl;
				//print output
				/*
					cout << word1 << " - " << word2 << endl;
					cout << "Score: " << counter << " Sequence(s): " ;
					cout << "\"" ;
				for (int i = 0; i < overlaps.size(); i++) {
					*commonSeq1 = overlaps[i];
					for(int i = counter-1; i>=0; i--) {
						cout<<commonSeq1[i]; 
					} 
				}
					cout << "\"" << endl;
					*/

				/*
				//ptritn conssss
				cout << commonSeq1 <<endl;
				cout << commonSeq2 <<endl;

				for(int i = counter-1; i>=0; i--) {
					cout<<commonSeq1[i]; 
				}
				cout<<endl;
				for(int j = counter-1; j>=0; j--) {
					cout<<commonSeq2[j];
				}
				cout<<endl;
				*/
			}

			//sort overlaps alphabetically
			sort(overlaps.begin(), overlaps.end());
			vector<string> sortedLetters;
			string newLetter = "";

			cout << word1 << " - " << word2 << endl;
			cout << "Score: " << counterScore << " Sequence(s): " ;
			for (int i = 0; i < (int)overlaps.size(); i++) {
				string overlapLetters = overlaps[i];
				
				for(int k = counter-1; k>=0; k--) {
					newLetter += overlapLetters[k];
				}

				sortedLetters.push_back(newLetter);
				newLetter = "";
			}

			//sort common sequences alphabetically
			sort(sortedLetters.begin(),sortedLetters.end());
			for(int i = 0; i < (int)sortedLetters.size(); i++) {
				cout << "\"" ;
				cout << sortedLetters[i];
				if(i == (int)(sortedLetters.size()-1)) {
					cout << "\"";
				} else {
					cout << "\" ";
				}
			}
			cout << endl;

			//write to output file
			//int score = counterScore + (mismatchCount * mismatchValue);
			ofstream output;
			output.open(outputFname, std::ios_base::app);
			output << word1 << " - " << word2 << endl;
			output << "Score: " << counterScore << " Sequence(s):" ;
			for(int i = 0; i < (int)sortedLetters.size(); i++) {
				if(counter != 0) {
					if (i == 0) {
						output << " \"" ;
					} else {
						output << "\"" ;
					}
					output << sortedLetters[i];
					if(i == (int)(sortedLetters.size()-1)) {
						output << "\"";
					} else {
						output << "\" ";
					}
				}
			}
			output << endl;
			output.close();

			overlaps.clear();

			mismatchCount = 0;
		}

		secondWordRef++;
		secondWordCounter = secondWordRef;

	}

	return 0;
}