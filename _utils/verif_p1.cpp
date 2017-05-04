/*
 * Copyright 2017
 * Eugen Vasilescu <eugen.vasilescu@cti.pub.ro>
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

void DIE(float points, std::string msg) {
	std::ofstream file_score("score.verif"), file_msg("output.verif");
	if (!file_score || !file_msg) {
		exit(0);
	}

	file_score << points << "\n";
	file_score.close();

	file_msg << msg << "\n";
	file_msg.close();

	exit(0);
}

bool check(std::string &alphabet, std::ifstream &file_in) {
	if (alphabet.size() != 26) {
		return false;
	}

	for (char ch = 'a'; ch <= 'z'; ch++) {
		int pos = alphabet.find(ch);
		if (pos == std::string::npos) {
			return false;
		}
	}

	int number_of_words;
	file_in >> number_of_words;

	std::string last_word, current_word;
	file_in >> last_word;
	for (int i = 2; i <= number_of_words; i++) {
		file_in >> current_word;

		int pos = 0;
		while (pos != last_word.size() && pos != current_word.size() &&
				last_word[pos] == current_word[pos]) {
			pos++;
		}

		if (pos == current_word.size()) {
			return false;
		}

		if (pos == last_word.size()) {
			continue;
		}

		std::size_t pos_last = alphabet.find(last_word[pos]);
		std::size_t pos_current = alphabet.find(current_word[pos]);

		if (pos_last == std::string::npos || pos_current == std::string::npos ||
				pos_last > pos_current) {
			return false;
		}
	}

	return true;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		DIE(0, "Usage ./verif problem_name test_points");
	}

	char in_name[100], out_name[100];
	strcpy(in_name, argv[1]);
	strcat(in_name, ".in");
	strcpy(out_name, argv[1]);
	strcat(out_name, ".out");

	float points = atof(argv[2]);

	std::ifstream file_in(in_name);
	std::ifstream file_out(out_name);
	std::ifstream file_ref("res.ok");

	if(!file_in) {
		DIE(0, "Nu exista fisierul de intrare");
	}

	if(!file_out) {
		DIE(0, "Nu exista fisier de output");
	}

	if(!file_ref) {
		DIE(0, "Nu exista fisierul de referinta");
	}

	std::string answer_out, answer_ref;
	file_ref >> answer_ref;
	file_out >> answer_out;
	file_ref.close();
	file_out.close();

	if ((answer_ref == "Imposibil" || answer_out == "Imposibil") &&
			answer_ref != answer_out) {
		file_in.close();
		DIE(0, std::string("WA: " + answer_out + " vs " + answer_ref));
	} else if (answer_ref == "Imposibil" && answer_out == "Imposibil") {
		file_in.close();
		DIE(points, std::string("OK"));
	}

	bool ok = check(answer_out, file_in);
	file_in.close();

	if (!ok) {
		DIE(0, std::string("WA: " + answer_out + " vs " + answer_ref));
	}

	DIE(points, std::string("OK"));

	return 0;
}
