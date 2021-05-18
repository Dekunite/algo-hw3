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

double checkSimilarity(char char1, char char2) {
	double result;

	if(char1 == char2) {
		//return match value
		result = match;
	}
	else {
		//return mismatch value if a not equals b
		result = mismatchValue;
	}

	return result;
}

double findMaximum(double boxes[]) {
	//start with the first box
	double max = boxes[0];
	index = 0;

	//check boxes for the max value
	for(int i = 1; i < 4; i++) {
		if(boxes[i] > max) {
			max = boxes[i];
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
	}
	if (argc > 2) {
		outputFname = argv[2];
	}
	if (argc > 3) {
		match = atoi(argv[3]);
	}
	if (argc > 4) {
		mismatchValue = atoi(argv[4]);
	}
	if (argc > 5) {
		gap = atoi(argv[5]);
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

	string word1, word2;
	int firstWordCounter = 0;
	int secondWordCounter = 0;
	int secondWordRef = 0;
	int counter;
	int counterScore;
	//vector to hold overlap letters
	vector<string> overlaps;
	vector<string> overlaps2;

	//loop through words
	for (;firstWordCounter < (int)words.size(); firstWordCounter++) {
		//skip over comparing the same words
		if(firstWordCounter == secondWordCounter) {
			secondWordCounter++;
		}
		for (; secondWordCounter < (int)words.size(); secondWordCounter++) {
			//assing words
			word1 = words[firstWordCounter];
			word2 = words[secondWordCounter];

			//word lengths
			int length1 = word1.length();
			int length2 = word2.length();

			//common sequences in word1 and word2
			char commonSeq1[length1 + length2 + 2];
			char commonSeq2[length1 + length2 + 2];

			//init matrix
			double matrix[length1+1][length2+1];
			for(int i = 0; i <= length1; i++) {
				for(int j = 0; j <= length2; j++) {
					//init as 0
					matrix[i][j]=0;
				}
			}

			//traceback array
			double traceback[4];
			int matrixI[length1+1][length2+1];
			int matrixJ[length1+1][length2+1];

			//filling the matrix
			for(int i = 1; i <= length1; i++) {
				for(int j = 1; j <= length2; j++) {
					traceback[0] = matrix[i-1][j-1] + checkSimilarity(word1[i-1], word2[j-1]);
					traceback[1] = matrix[i-1][j] + gap;
					traceback[2] = matrix[i][j-1] + gap;
					traceback[3] = 0;
					//find the max score in the traceback aray
					matrix[i][j] = findMaximum(traceback);

					//fill out matrix accordingly
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

					if(nextI == currentI) {
						commonSeq1[counter] = '-';
					} else {
						//add matching letter to common sequence
						commonSeq1[counter] = word1[currentI-1];
					}

					if(nextJ == currentJ) {
						commonSeq2[counter] = '-';
					} else {
						//add matching letter to common sequence
						commonSeq2[counter] = word2[currentJ-1];
					}

					//change nextI and nextJ values to prevent infinite loop
					if (nextI == 0) {
						nextI = -1;
					} else if (nextJ == 0) {
						nextJ = -1;
					} else {
						//continue iteration by getting next elements in line
						currentI = nextI;
						currentJ = nextJ;
						nextI = matrixI[currentI][currentJ];
						nextJ = matrixJ[currentI][currentJ];
					}
					counter++;
					counterScore += match;
				}
				string temp = commonSeq1;
				//skip over the same common sequences, dont push them to vector
				if (!(find(overlaps.begin(), overlaps.end(), temp) != overlaps.end())) {
					overlaps.push_back(commonSeq1);
				}
				temp = commonSeq2;
				if (!(find(overlaps2.begin(), overlaps2.end(), temp) != overlaps2.end())) {
					overlaps2.push_back(commonSeq2);
				}

			}

			//sort overlaps alphabetically
			sort(overlaps.begin(), overlaps.end());
			sort(overlaps2.begin(), overlaps2.end());

			//calculate the mismatch count between words for score calculation
			for (int i=counter-1; i>=0; i--) {
				if (commonSeq1[i] != commonSeq2[i]) {
					mismatchCount++;
				}
			}
			counterScore += mismatchCount * mismatchValue;
			counterScore -= mismatchCount * match;

			vector<string> sortedLetters;
			string newLetter = "";

			//cout << word1 << " - " << word2 << endl;
			//cout << "Score: " << counterScore << " Sequence(s): " ;

			//select out letters from common sequences
			for (int i = 0; i < (int)overlaps.size(); i++) {
				string overlapLetters = overlaps[i];
				
				for(int k = counter-1; k>=0; k--) {
					newLetter += overlapLetters[k];
				}

				sortedLetters.push_back(newLetter);
				newLetter = "";
			}

			//sort selected letters alphabetically
			sort(sortedLetters.begin(),sortedLetters.end());

			/*
			//UNUSED PRINT
			for(int i = 0; i < (int)sortedLetters.size(); i++) {
				//cout << "\"" ;
				//cout << sortedLetters[i];
				//if(i == (int)(sortedLetters.size()-1)) {
					//cout << "\"";
				//} else {
					//cout << "\" ";
				//}
			}
			//cout << endl;
			*/

			//write to output file
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

			//clear vectors
			overlaps.clear();
			overlaps2.clear();

			//reset mismatch count
			mismatchCount = 0;
		}

		secondWordRef++;
		secondWordCounter = secondWordRef;

	}

	return 0;
}