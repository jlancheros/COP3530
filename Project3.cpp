#include <iostream>
#include <vector>
#include <map>
#include<algorithm>
#include<string>
#include<iterator>
#include<set>
#include<random>
#include<chrono>
using namespace std;
using  namespace std::chrono;

class Node
{
public:
	vector<int> answers;
	vector<int> preferences{ 1,2,3,4,5,6,7,8,9,10 };
};

//Implementation using Maps
class Staisle_Maps
{
public:

	map<int, Node*> Staisle; //Stores all randomly generated user data
	map<int, string> products; //Map to translate ints into our products
	vector<vector<int>> Sorts; //Stores (as vectors) the results of each sort
	vector<string> questions;

	void generateData(int entries)
	{
		products[1] = "Air Jordans";
		products[2] = "Vans Slip-on";
		products[3] = "New Balance Classic";
		products[4] = "Nike Free Run";
		products[5] = "Adidas Superstar";
		products[6] = "Puma Suede";
		products[7] = "Reebok Club";
		products[8] = "Asics Gel-Lyte";
		products[9] = "Converse Chuck Taylor";
		products[10] = "Crocs";

		int userNum = 0;
		for (int i = 0; i < entries; ++i)
		{
			++userNum; //Index of fakeUser
			Node* fakeUser = new Node; //Create new fake user

			for (int j = 0; j < 5; ++j) //Answer the 5 questions randomly
			{
				fakeUser->answers.push_back(rand() % 2);
			}

			shuffleVector(fakeUser->preferences);  //Randomly shuffle the fake user's preferences
			Staisle[userNum] = fakeUser; //Add fakeUser to the map
		}
	}

	vector<int> askQuestions(Node& person)
	{
		questions.push_back("Do you enjoy spicy food?");
		questions.push_back("Is camping something you would like? ");
		questions.push_back("Do you like to dance at parties?");
		questions.push_back("Are you a fan of horror movies?");
		questions.push_back("Is Hip-Hop your favorite genre of music?");

		int response;
		for (unsigned int i = 0; i < questions.size(); i++)
		{
			response = -1;
			cout << "Question " << i + 1 << ": " << questions[i] << endl;
			cout << "(Enter 1 for Yes or 0 for No) " << endl;
			cin >> response;
			while (response != 1 && response != 0)
			{
				cout << "Oops, that's not a valid response." << endl;
				cout << "Enter 1 for Yes or 0 for No" << endl;
				cin >> response;
			}
			person.answers.push_back(response);
			cout << endl;
		}

		return person.answers;
	}

	void printAllData()
	{
		map<int, Node*>::iterator itr;

		for (itr = Staisle.begin(); itr != Staisle.end(); ++itr)
		{
			for (int j = 0; j < 5; ++j) {
				cout << itr->second->answers[j] << " ";
			}
			cout << endl;
			for (int j = 0; j < 10; ++j) {
				cout << itr->second->preferences[j] << " ";
			}
			cout << endl;
		}
	}

	void Sort(vector<int> yourAnswers, int questionNum)
	{
		vector <Node*> matchedAnswers;
		map<int, Node*>::iterator itr;//Staisle map iterator
		map<int, int> recommended;
		vector<int> sortedPreferences;

		for (itr = Staisle.begin(); itr != Staisle.end(); ++itr) {
			//Compare fake users that have same answers as real user and store in vector
			if (yourAnswers.at(questionNum) == itr->second->answers.at(questionNum))
				matchedAnswers.push_back(itr->second);
		}

		for (int j = 0; j < 10; ++j)
		{
			//Populate "recommended" map with the frequencies that a preference appears at the jth index in the "preferences" vector of fake users
			for (unsigned int i = 0; i < matchedAnswers.size(); i++)
			{
				if (recommended.find(matchedAnswers[i]->preferences.at(j)) == recommended.end()) {
					recommended.emplace(matchedAnswers[i]->preferences.at(j), 1);
				}
				else if (recommended.find(matchedAnswers[i]->preferences.at(j)) != recommended.end()) {
					recommended[matchedAnswers[i]->preferences.at(j)]++;
				}
			}
			//cout << endl;
			int max = 0;
			int favorite;
			for (auto iter = recommended.begin(); iter != recommended.end(); iter++)  //Find the highest frequency, push that index into the "sortedPreferences" vector
			{
				if (iter->second > max) {
					max = iter->second;
					favorite = iter->first;
				}
			}
			sortedPreferences.push_back(favorite);
			recommended.clear(); //Clear recommended so we have a clean slate for the next preference
		}
		Sorts.push_back(sortedPreferences); //Done sorting... Push back entire vector in "Sorts"

	}

	int chooseSort()
	{
		int response;

		cout << "Which question would you like to sort by?" << endl << endl;
		cout << "1. Do you enjoy spicy food?" << endl;
		cout << "2. Is camping something you would like?" << endl;
		cout << "3. Do you like to dance at parties?" << endl;
		cout << "4. Are you a fan of horror movies?" << endl;
		cout << "5. Is Hip-Hop your favorite genre of music?" << endl;
		cout << "6. Overall" << endl;
		cout << "0. Exit" << endl;

		cin >> response;

		cout << endl;

		while (response < 0 || response > 6)
		{
			cout << "Oops, that's not a valid response." << endl;
			cin >> response;
		}

		if (response == 0)
			return 0;

		set<string> printed;
		string item;
		int printNum = 1;
		int index = 0;
		bool check;

		if (response < 6)
			cout << "Recommended products based on: " << questions.at(response - 1) << endl << endl;
		else
			cout << "Recommended products based on: Overall answers " << endl << endl;

		for (int i = 0; i < 5; ++i) //This loop simply prints out our products from the appropriate vector in "Sorts".  It skips an element in the vector if it repeats
		{

			check = false;
			item = products[Sorts[response - 1][index]];

			while (check == false)
			{
				if (printed.find(item) == printed.end()) //If item has not been printed yet
				{
					cout << printNum << ": " << item << endl;
					printed.insert(item);
					check = true;
					++index;
				}
				else
				{
					++index;
					item = products[Sorts[response - 1][index]];
				}
			}
			++printNum;
		}
		cout << endl << endl;
		return response;
	}

	void masterSort()
	{
		map<int, int> masterSort;
		vector<int> masterPreferences;

		for (unsigned int i = 0; i < Sorts.size(); ++i) //Make a map with the sums of all 5 sorts, from the top 5 preferences in each sort
		{
			for (int j = 0; j < 5; ++j)
			{
				if (masterSort.find(Sorts[i][j]) == masterSort.end())
					masterSort[Sorts[i][j]] = 1;

				else
					masterSort[Sorts[i][j]]++;
			}
		}

		for (int i = 0; i < 10; ++i) //Get highest preference, push it into a vector, remove it from the map, repeat
		{
			int max = 0;
			int favorite;
			for (auto iter = masterSort.begin(); iter != masterSort.end(); ++iter)
			{
				if (iter->second > max)
				{
					max = iter->second;
					favorite = iter->first;
				}
			}
			masterPreferences.push_back(favorite);
			masterSort.erase(favorite);
		}
		Sorts.push_back(masterPreferences);
	}

private:
	void shuffleVector(vector<int>& vector)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //Gets random seed value based on system clock
		shuffle(vector.begin(), vector.end(), std::default_random_engine(seed)); //Uses random seed to shuffle vector
	}
};

//Implementation using an Ordered Set
class Staisle_Sets
{
public:

	set<Node*> Staisle; //Stores all randomly generated user data
	set<pair<int, string>> products; //Map to translate ints into our products
	vector<vector<int>> Sorts; //Stores (as vectors) the results of each sort
	vector<string> questions;

	void generateData(int entries)
	{
		products.insert(make_pair(1, "Air Jordans"));
		products.insert(make_pair(2, "Vans Slip-on"));
		products.insert(make_pair(3, "New Balance Classic"));
		products.insert(make_pair(4, "Nike Free Run"));
		products.insert(make_pair(5, "Adidas Superstar"));
		products.insert(make_pair(6, "Puma Suede"));
		products.insert(make_pair(7, "Reebok Club"));
		products.insert(make_pair(8, "Asics Gel-Lyte"));
		products.insert(make_pair(9, "Converse Chuck Taylor"));
		products.insert(make_pair(10, "Crocs"));

		int userNum = 0;
		for (int i = 0; i < entries; ++i)
		{
			++userNum; //Index of fakeUser
			Node* fakeUser = new Node; //Create new fake user

			for (int j = 0; j < 5; ++j) //Answer the 5 questions randomly
			{
				fakeUser->answers.push_back(rand() % 2);
			}

			shuffleVector(fakeUser->preferences);  //Randomly shuffle the fake user's preferences
			Staisle.insert(fakeUser); //Add fakeUser to the set
		}
	}

	vector<int> askQuestions(Node& person)
	{
		questions.push_back("Do you enjoy spicy food?");
		questions.push_back("Is camping something you would like? ");
		questions.push_back("Do you like to dance at parties?");
		questions.push_back("Are you a fan of horror movies?");
		questions.push_back("Is Hip-Hop your favorite genre of music?");

		int response;
		for (unsigned int i = 0; i < questions.size(); i++)
		{
			response = -1;
			cout << "Question " << i + 1 << ": " << questions[i] << endl;
			cout << "(Enter 1 for Yes or 0 for No) " << endl;
			cin >> response;
			while (response != 1 && response != 0)
			{
				cout << "Oops, that's not a valid response." << endl;
				cout << "Enter 1 for Yes or 0 for No" << endl;
				cin >> response;
			}
			person.answers.push_back(response);
			cout << endl;
		}

		return person.answers;
	}

	void Sort(vector<int> yourAnswers, int questionNum)
	{
		vector<Node*> matchedAnswers;
		set<Node*>::iterator itr;//Staisle iterator
		set<pair<int, int>> recommended; //Index
		set<int> found;
		int frequency;
		vector<int> sortedPreferences;

		for (itr = Staisle.begin(); itr != Staisle.end(); ++itr)
		{
			//Compare fake users that have same answers as real user and store it
			if (yourAnswers.at(questionNum) == (*itr)->answers.at(questionNum))
				matchedAnswers.push_back(*itr); //Push_back the node
		}

		for (int j = 0; j < 10; ++j)
		{
			//Populate "recommended" set with the frequencies that a preference appears at the jth index in the "preferences" vector of fake users
			for (unsigned int i = 0; i < matchedAnswers.size(); i++)
			{

				if (found.find(j) == found.end()) //Preference ha no been counted yet
				{
					found.insert(j);
					recommended.insert(make_pair(matchedAnswers.at(i)->preferences.at(j), 1));
				}
				else //Preference has been counted at least once
				{
					for (auto itr = recommended.begin(); itr != recommended.end(); ++itr)
					{
						if (itr->first == j)
						{
							int frequency = itr->second;
							recommended.erase(itr);
							recommended.insert(make_pair(j, frequency + 1));
							break;
						}
					}
				}
			}

			//cout << endl;
			int max = 0;
			int favorite;
			for (auto iter = recommended.begin(); iter != recommended.end(); iter++)  //Find the highest frequency, push that index into the "sortedPreferences" vector
			{
				if (iter->second > max) {
					max = iter->second;
					favorite = iter->first;
				}
			}
			sortedPreferences.push_back(favorite);
			recommended.clear(); //Clear recommended so we have a clean slate for the next preference
			found.clear(); //Clear found as well
		}
		Sorts.push_back(sortedPreferences); //Done sorting... Push back entire vector in "Sorts"

	}

	int chooseSort()
	{
		int response;

		cout << "Which question would you like to sort by?" << endl << endl;
		cout << "1. Do you enjoy spicy food?" << endl;
		cout << "2. Is camping something you would like?" << endl;
		cout << "3. Do you like to dance at parties?" << endl;
		cout << "4. Are you a fan of horror movies?" << endl;
		cout << "5. Is Hip-Hop your favorite genre of music?" << endl;
		cout << "6. Overall" << endl;
		cout << "0. Exit" << endl;

		cin >> response;

		cout << endl;

		while (response < 0 || response > 6)
		{
			cout << "Oops, that's not a valid response." << endl;
			cin >> response;
		}

		if (response == 0)
			return 0;

		set<string> printed;
		string item;
		int printNum = 1;
		int index = 0;
		bool check;

		if (response < 6)
			cout << "Recommended products based on: " << questions.at(response - 1) << endl << endl;
		else
			cout << "Recommended products based on: Overall answers " << endl << endl;

		for (int i = 0; i < 5; ++i) //This loop simply prints out our products from the appropriate vector in "Sorts".  It skips an element in the vector if it repeats
		{

			check = false;
			for (auto itr = products.begin(); itr != products.end(); ++itr)
			{
				int output = Sorts[response - 1][index];
				if (itr->first == output)
				{
					item = itr->second;
				}
			}
			while (check == false)
			{
				if (printed.find(item) == printed.end()) //If item has not been printed yet
				{
					cout << printNum << ": " << item << endl;
					printed.insert(item);
					check = true;
					++index;
				}
				else
				{
					++index;
					for (auto itr = products.begin(); itr != products.end(); ++itr)
					{
						int output = Sorts[response - 1][index];
						if (itr->first == output)
						{
							item = itr->second;
						}
					}
				}
			}
			++printNum;
		}
		cout << endl << endl;
		return response;
	}

	void masterSort()
	{
		map<int, int> masterSort;
		vector<int> masterPreferences;

		for (unsigned int i = 0; i < Sorts.size(); ++i) //Make a map with the sums of all 5 sorts, from the top 5 preferences in each sort
		{
			for (int j = 0; j < 5; ++j)
			{
				if (masterSort.find(Sorts[i][j]) == masterSort.end())
					masterSort[Sorts[i][j]] = 1;
				else
					masterSort[Sorts[i][j]]++;
			}
		}

		for (int i = 0; i < 10; ++i) //Get highest preference, push it into a vector, remove it from the map, repeat
		{
			int max = 0;
			int favorite;
			for (auto iter = masterSort.begin(); iter != masterSort.end(); ++iter)
			{
				if (iter->second > max)
				{
					max = iter->second;
					favorite = iter->first;
				}
			}
			masterPreferences.push_back(favorite);
			masterSort.erase(favorite);
		}
		Sorts.push_back(masterPreferences);
	}

private:
	void shuffleVector(vector<int>& vector)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //Gets random seed value based on system clock
		shuffle(vector.begin(), vector.end(), std::default_random_engine(seed)); //Uses random seed to shuffle vector
	}
};

int main() {

	cout << "Welcome to Staisle! " << endl << endl;

	cout << "Staisle recommends products to you based on your personality, tastes, and preferences" << endl << endl << endl;
	cout << "Answer the following questions and we'll build you your own personal Staisle: " << endl;

	int choice = -1;

	cout << "Select either Map or Set implementation: " << endl;
	cout << "1. Maps" << endl;
	cout << "2. Sets" << endl;
	cin >> choice;

	while (choice < 0 || choice > 2)
	{
		cout << "Invalid choice! Please try again: " << endl;
		cin >> choice;
	}

	if (choice == 1)
	{

		Staisle_Maps myStaisle;

		int entries = 100000; //# of fake users

		myStaisle.generateData(entries); //Enter # of fake users here

		Node person;
		myStaisle.askQuestions(person);

		cout << "Your answers: ";
		for (unsigned int i = 0; i < person.answers.size(); ++i)
		{
			cout << person.answers[i] << " ";
		}
		cout << endl << endl;

		cout << endl;

		cout << "Loading your personal Staisle!" << endl << endl;


		auto start = high_resolution_clock::now(); //Initialize clock to measure time execution of Sort function

		for (int i = 0; i < 5; i++) //Sort all answers of the fake users that matched the answers for each of the questions
		{
			myStaisle.Sort(person.answers, i);
		}

		auto stop = high_resolution_clock::now(); // Stop the clock after the function ended
		auto duration = duration_cast<microseconds>(stop - start); //Calculate the time execution of the function
		cout << "Sort duration: " << duration.count() << " microseconds" << endl;


		start = high_resolution_clock::now();
		myStaisle.masterSort(); //Make a master sort with the products with higher frequency
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Master Sort duration: " << duration.count() << " microseconds" << endl << endl;


		cout << "All done! You can now sort through our products based on the questions you've answered!" << endl;
		cout << "The results are based on the preferences of people who have answered similarly to you" << endl << endl;
		cout << "Based on your overall answers, we think you would like these products the best" << endl;

		for (int i = 0; i < 5; ++i)
		{
			cout << i + 1 << ": " << myStaisle.products[myStaisle.Sorts[5][i]] << endl;
		}
		cout << endl;

		cout << "You can also sort based on any of the questions you've answered!" << endl << endl;

		int action = -1;
		while (action != 0)
		{
			start = high_resolution_clock::now();
			action = myStaisle.chooseSort();
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);
			cout << "Choose Sort duration: " << duration.count() << " microseconds" << endl << endl;
		}
	}
	/*************************************************** Staisle IMPLEMENTATION USING  SETS *************************************************/
	else if (choice == 2)
	{
		Staisle_Sets newStaisle;

		int entry = 100000; //# of fake users

		newStaisle.generateData(entry); //Enter # of fake users here

		Node p;
		newStaisle.askQuestions(p);

		cout << "Your answers: ";
		for (unsigned int i = 0; i < p.answers.size(); ++i)
		{
			cout << p.answers[i] << " ";
		}
		cout << endl << endl << endl;

		cout << "Loading your personal Staisle!" << endl << endl;

		auto start = high_resolution_clock::now(); //Initialize clock to measure time execution of Sort function
		for (int i = 0; i < 5; i++) //Sort all answers of the fake users that matched the answers for each of the questions
		{
			newStaisle.Sort(p.answers, i);
		}

		auto stop = high_resolution_clock::now(); // Stop the clock after the function ended
		auto duration = duration_cast<microseconds>(stop - start); //Calculate the time execution of the function
		cout << "Sort duration: " << duration.count() << " microseconds" << endl;

		start = high_resolution_clock::now();
		newStaisle.masterSort();
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Master Sort duration: " << duration.count() << " microseconds" << endl << endl;


		cout << "All done!  You can now sort through our products based on the questions you've answered!" << endl;
		cout << "The results are based on the preferences of people who have answered similarly to you" << endl << endl;
		cout << "Based on your overall answers, we think you would like these products the best" << endl;

		for (int i = 0; i < 5; ++i)
		{
			cout << i + 1 << ": ";
			int output = newStaisle.Sorts[5][i];
			for (auto itr = newStaisle.products.begin(); itr != newStaisle.products.end(); ++itr)
			{
				if (itr->first == output)
				{
					cout << itr->second << endl;
					break;
				}
			}
		}
		cout << endl;

		cout << "You can also sort based on any of the questions you've answered!" << endl << endl;

		int action = -1;
		while (action != 0)
		{
			start = high_resolution_clock::now();
			action = newStaisle.chooseSort();
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);
			cout << "Choose Sort duration: " << duration.count() << " microseconds" << endl << endl;
		}
	}

	cout << "We hope our recommendations were of your liking. Thank you for using Staisle!" << endl;

	return 0;
}

