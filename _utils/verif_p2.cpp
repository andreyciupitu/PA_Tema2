/*
 * Copyright 2017
 * Eugen Vasilescu <eugen.vasilescu@cti.pub.ro>
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
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

int main(int argc, char **argv) {
	if(argc != 3) {
		DIE(0, "Usage ./verif problem_name test_points");
	}

	char out_name[100];
	strcpy(out_name, argv[1]);
	strcat(out_name, ".out");

	float points = atof(argv[2]);

	std::ifstream file_out(out_name);
	std::ifstream file_ref("res.ok");

	if(!file_out) {
		DIE(0, "Nu exista fisier de output");
	}

	if(!file_ref) {
		DIE(0, "Nu exista fisierul de referinta");
	}

	std::string answer_out, answer_ref;
	file_out >> answer_out;
	file_ref >> answer_ref;
	file_out.close();
	file_ref.close();

	if (answer_ref != answer_out) {
		DIE(0, std::string("WA: " + answer_out + " vs " + answer_ref));
	}
	DIE(points, std::string("OK"));

	return 0;
}
