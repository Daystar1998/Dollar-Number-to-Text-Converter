
/******************************************************************************
	Copyright 2018 Matthew Day

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	https://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
******************************************************************************/

/******************************************************************************
	File Name: converter.cpp

	Author: Matthew Day

	Description:
		This program takes in a number between 0 and 1 billion inclusive and
		converts it to a text dollar amount.
******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

/******************************************************************************
	Function Name: roundCents

	Des:
		Rounds cents to 2 places

	Params:
		cents - type string &, the amount of cents and should be at least 3
			characters long

	Returns:
		type int, the amount of cents after rounding
******************************************************************************/
int roundCents(string &cents) {

	if (cents.length() < 3)
		throw invalid_argument("Cents should be at least 3 characters long");

	int result = 0;

	// Round centsToken to 2 digits and convert to integer
	if (cents[2] - '0' >= 5) {

		// Round up
		result = stoi(cents.substr(0, 2)) + 1;
	} else {

		// Round down
		result = stoi(cents.substr(0, 2));
	}

	return result;
}

/******************************************************************************
	Function Name: splitByDecimalPoint

	Des:
		Splits the amount by the dollars and cents

	Params:
		amount - type string &, input the total amount of money before it has been split
		dollarsToken - type string &, output the dollar section of the amount
		cents - type int &, output the cents in the amount
******************************************************************************/
void splitByDecimalPoint(string &amount, string &dollarsToken, int &cents) {

	string centsToken = "";
	unsigned int centsStart = -1;

	// Initialize dollarsToken to empty string
	dollarsToken = "";
	cents = 0;

	// Loop to get the dollar amount
	for (unsigned int i = 0; i < amount.length(); i++) {

		// Check if the current character is a decimal
		if (amount[i] == '.') {

			// Add one to skip decimal point
			centsStart = i + 1;
			break;
		}

		dollarsToken += amount[i];
	}

	// Check if any centsStart was ever changed from the default
	if (centsStart != -1) {

		// Loop to get the decimal value up to 3 places
		for (unsigned int i = centsStart; i <= centsStart + 2; i++) {

			if (i < amount.length())
				centsToken += amount[i];
			else
				centsToken += '0';
		}

		// Round cents to 2 places
		cents = roundCents(centsToken);

		// Check if cents rounded up to 1 dollar
		if (cents >= 100) {

			// Add 1 to the total dollar amount
			dollarsToken = to_string(stoi(dollarsToken) + 1);
			// Reset cents to 0
			cents = 0;
		}
	}
}

/******************************************************************************
	Function Name: processOnes

	Des:
		Process the number to get the word naming that number

	Params:
		number - type char, the number to be processed

	Returns:
		type string, the name for the input number
******************************************************************************/
string processOnes(char number) {

	string result;

	switch (number) {

		case '0':
			result = "";
			break;
		case '1':
			result = "One";
			break;
		case '2':
			result = "Two";
			break;
		case '3':
			result = "Three";
			break;
		case '4':
			result = "Four";
			break;
		case '5':
			result = "Five";
			break;
		case '6':
			result = "Six";
			break;
		case '7':
			result = "Seven";
			break;
		case '8':
			result = "Eight";
			break;
		case '9':
			result = "Nine";
			break;
		default:
			throw invalid_argument(number + " is invalid");
	}

	return result;
}

/******************************************************************************
	Function Name: processHundreds

	Des:
		Converts hundreds of dollars to words

	Params:
		dollars - type string &, the dollar amount to be converted

	Returns:
		type string, the dollar amount in word form
******************************************************************************/
string processHundreds(string &amount) {

	string result = "";
	int current = 0;

	if (amount.length() == 3) {

		// Process the hundreds place
		if (amount[0] != '0')
			result += processOnes(amount[0]) + " Hundred";

		current++;
	}

	if (amount.length() >= 2) {

		if (amount[current] == '1') {

			// Check if anything other than 0 was in hundreds place then separate it with a space
			if (result != "")
				result += ' ';

			// Process the teens
			switch (amount[current + 1]) {

				case '0':
					result += "Ten";
					break;
				case '1':
					result += "Eleven";
					break;
				case '2':
					result += "Twelve";
					break;
				case '3':
					result += "Thirteen";
					break;
				case '4':
					result += "Fourteen";
					break;
				case '5':
					result += "Fifteen";
					break;
				case '6':
					result += "Sixteen";
					break;
				case '7':
					result += "Seventeen";
					break;
				case '8':
					result += "Eighteen";
					break;
				case '9':
					result += "Nineteen";
					break;
			}
		} else if (amount[current] != '0') {

			// Check if anything other than 0 was in hundreds place then separate it with a space
			if (result != "")
				result += ' ';

			// Process tens place
			switch (amount[current]) {

				case '2':
					result += "Twenty";
					break;
				case '3':
					result += "Thirty";
					break;
				case '4':
					result += "Forty";
					break;
				case '5':
					result += "Fifty";
					break;
				case '6':
					result += "Sixty";
					break;
				case '7':
					result += "Seventy";
					break;
				case '8':
					result += "Eighty";
					break;
				case '9':
					result += "Ninety";
					break;
			}

			// Process ones place if it is not 0
			if (amount[current + 1] != '0')
				result += " " + processOnes(amount[current + 1]);
		// If tens does not exist then process ones place if it is not 0
		} else if (amount[current + 1] != '0') {

			// Check if anything other than 0 was in hundreds place then separate it with a space
			if (result != "")
				result += ' ';

			result += processOnes(amount[current + 1]);
		}
	// If tens does not exist then process ones place if it is not 0
	} else if (amount[current] != '0') {

		// Check if anything other than 0 was in hundreds place then separate it with a space
		if (result != "")
			result += ' ';

		result += processOnes(amount[current]);
	}

	return result;
}

/******************************************************************************
	Function Name: processDollars

	Des:
		Convert the dollar amount to word form

	Params:
		dollars - type string &, the dollar amount to be converted

	Returns:
		type string, the dollar amount in word form
******************************************************************************/
string processDollars(string &dollars) {

	string result = "";
	int start = 0;

	if (dollars == "0" || dollars == "") {

		result = "No Dollars";
	} else if (dollars == "1") {

		result = "One Dollar";
	} else {

		// Billions place
		if (dollars.length() > 9) {

			// Assume to be 1 billion since that is the maximum amount
			result = "One Billion";
		// Millions place
		} else if (dollars.length() > 6) {

			// The length of the starting section of the number
			// Example: dollars = 1234, leadingNumberLength equals 1
			int leadingNumberLength = dollars.length() % 3;

			if (leadingNumberLength == 0)
				leadingNumberLength = 3;

			// Get the values in the millions place
			string millionsPlace = dollars.substr(0, leadingNumberLength);

			if (millionsPlace != "")
				result += processHundreds(millionsPlace) + " Million";

			// Get the value in the thousands place
			string thousandsPlace = dollars.substr(leadingNumberLength, 3);

			if (thousandsPlace != "000")
				result += ' ' + processHundreds(thousandsPlace) + " Thousand";

			// Get the value in the hundreds place
			string hundredsPlace = dollars.substr(leadingNumberLength + 3, 3);

			if (hundredsPlace != "000")
				result += ' ' + processHundreds(hundredsPlace);
		// Thousands place
		} else if (dollars.length() > 3) {

			// The length of the starting section of the number
			// Example: dollars = 1234, leadingNumberLength equals 1
			int leadingNumberLength = dollars.length() % 3;

			if (leadingNumberLength == 0)
				leadingNumberLength = 3;

			// Get the value in the thousands place
			string thousandsPlace = dollars.substr(0, leadingNumberLength);

			if (thousandsPlace != "")
				result += processHundreds(thousandsPlace) + " Thousand";

			// Get the value in the hundreds place
			string hundredsPlace = dollars.substr(leadingNumberLength, 3);

			if (hundredsPlace != "000")
				result += ' ' + processHundreds(hundredsPlace);
		// Hundreds place
		} else {

			result += processHundreds(dollars);
		}

		result += " Dollars";
	}

	return result;
}

/******************************************************************************
	Function Name: printAmount

	Des:
		Prints the amount to the console in text form

	Params:
		dollars - type string, the dollar amount in text format
		cents - type int, the amount of cents
******************************************************************************/
void printAmount(string dollars, int cents) {

	cout << dollars << " and ";

	if (cents == 0)
		cout << "No Cents" << endl;
	else if (cents == 1)
		cout << "1 Cent" << endl;
	else
		cout << cents << " Cents" << endl;
}

/******************************************************************************
	Main Function
******************************************************************************/
int main() {

	string amount;

	do {

		cout << "Amount? ";
		cin >> amount;
	} while (stoi(amount) < 0 || stoi(amount) > 1000000000);

	string dollars = "";
	int cents;

	while (amount != "0") {

		splitByDecimalPoint(amount, dollars, cents);

		dollars = processDollars(dollars);

		printAmount(dollars, cents);

		do {

			cout << "Amount? ";
			cin >> amount;
		} while (stoi(amount) < 0 || stoi(amount) > 1000000000);
	}
}