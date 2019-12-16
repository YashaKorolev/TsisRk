#include<iostream>
#include<vector>
#include<cmath>
#include <ctime>
#include <algorithm>
#include <iomanip>
#define A -2.2
#define B 2.2
#define PMUT 0.25



double f(double, double);
struct Osob
{
	double x;
	double y;
	double fxy;
	Osob(double in_x = 0, double in_y = 0) {
		x = in_x;
		y = in_y;
		fxy = f(x, y);
	}

	bool operator==(Osob osob) const {
		if (osob.x == x && osob.y == y)
			return true;
		else
			return false;
	}
};

struct Population {
	std::vector<Osob> osobs;

	bool mutation = false;

	Osob better_osob() {
		Osob res;
		if (osobs.size() > 0) {
			res = osobs[0];
			for (std::vector<Osob>::iterator osob = osobs.begin(); osob != osobs.end(); osob++) {
				if (res.fxy < osob->fxy) {
					res = *osob;
				}
			}
		}
		return res;
	}

	double middle() {
		double res = 0.0;
		for (std::vector<Osob>::iterator osob = osobs.begin(); osob != osobs.end(); osob++) {
			res += osob->fxy;
		}
		return res / osobs.size();
	}

	bool operator==(Population population) const {

		std::sort(population.osobs.begin(), population.osobs.end(), [](Osob os1, Osob os2)-> bool {return os1.fxy < os2.fxy; });
		std::vector<Osob> osobs1 = osobs;

		std::sort(osobs1.begin(), osobs1.end(), [](Osob os1, Osob os2)-> bool {return os1.fxy < os2.fxy; });

		if (population.osobs.size() != osobs1.size()) {
			return false;
		}

		for (int i = 0; i < osobs.size(); i++) {
			if (!(population.osobs[i] == osobs1[i])) {
				return false;
			}
		}

		return true;
	}

};

double f(double x, double y)
{
	return sin(x) + exp(-(x * x) - (y * y));
}

double randf(double a, double b) {
	double r01 = (double)rand() / RAND_MAX;
	return a + r01 * (b - a);
}


// flag определякт нужно ли выводить парамтры параметры
//double max = 0.0, double middle = 0.0, bool mutation
void print_osob(int id, Osob osob, double max = 0.0, double middle = 0.0, bool mutation = false, bool flag = false) {

	if (flag) {
		std::cout << std::setw(5) << id << " | "
			<< std::fixed
			<< std::setw(10) << osob.x << " | "
			<< std::setw(10) << osob.y << " | "
			<< std::setw(10) << osob.fxy << " | "
			<< std::setw(10) << max << " | "
			<< std::setw(10) << middle << " | "
			<< std::setw(10) << (mutation ? "+" : "-") << std::endl;
	}
	else {
		std::cout << std::setw(5) << " " << " | "
			<< std::fixed
			<< std::setw(10) << osob.x << " | "
			<< std::setw(10) << osob.y << " | "
			<< std::setw(10) << osob.fxy << " | "
			<< std::setw(10) << " " << " | "
			<< std::setw(10) << " " << " | "
			<< std::setw(10) << " " << std::endl;
	}

}

void print_population(Population population, int counter) {

	bool is_first = true;
	for (auto osob : population.osobs) {
		if (is_first) {
			print_osob(counter, osob, population.better_osob().fxy, population.middle(), population.mutation, true);
			is_first = false;
		}
		else {
			print_osob(counter, osob);
		}
		
	}
	//std::cout << std::endl;
}

Osob POsob(Population population, double p) {


	// При вероятности 0 функция не работает
	// Контроль вести при передаче вероятности

	double s = 0.0;
	for (std::vector<Osob>::iterator osob = population.osobs.begin(); osob != population.osobs.end(); osob++) {
		s += abs(osob->fxy);
	}

	std::sort(population.osobs.begin(), population.osobs.end(), [](Osob os1, Osob os2)-> bool {return abs(os1.fxy) < abs(os2.fxy); });

	for (auto osob : population.osobs) {
		if (p <= abs(osob.fxy) / s) {
			return osob;
		}
	}

	return population.osobs.back();
}


int main() {

	srand(time(NULL));

	Population population;

	std::vector<Osob> osobs;
	osobs.push_back(Osob(randf(A, B), randf(A, B)));
	osobs.push_back(Osob(randf(A, B), randf(A, B)));
	osobs.push_back(Osob(randf(A, B), randf(A, B)));
	osobs.push_back(Osob(randf(A, B), randf(A, B)));

	population.osobs = osobs;

	// Шапка
	std::cout << "________________________________________________________________________________________" << std::endl;
	std::cout << " NUM |      X     |      Y     |    FIT     |   MAX_FIT  |   MID_FIT  |       MUTATION  " << std::endl;
	std::cout << "________________________________________________________________________________________" << std::endl;

	int i;
	for (i = 0; i < 100; i++) {

		print_population(population, i);
		std::cout << "________________________________________________________________________________________" << std::endl;

		double p;

		p = randf(0., 1.);
		Osob osob1 = POsob(population, p);

		p = randf(0., 1.);
		Osob osob2 = POsob(population, p);

		Osob osob12(osob1.x, osob2.y);
		Osob osob21(osob2.x, osob1.y);

		//мутация
		bool is_mutation = false;
		//Берем случ от 0 до 1 для osob12
		double p12 = randf(0., 1.);
		//проверяем куда он попал.
		if (p12 <= PMUT) {
			is_mutation = true;

			osob12.x = osob12.x + randf(-1., 1.);
			if (osob12.x > 2.2)
				osob12.x = 2.2;
			if (osob12.x < -2.2)
				osob12.x = -2.2;
			osob12.y = osob12.y + randf(-1., 1.);
			if (osob12.y > 2.2)
				osob12.y = 2.2;
			if (osob12.y < -2.2)
				osob12.y = -2.2;
		}

		//Берем случ от 0 до 1 для osob21
		double p21 = randf(0., 1.);
		//проверяем куда он попал.
		if (p21 <= PMUT) {
			is_mutation = true;
			
				osob21.x = osob21.x + randf(-1., 1.);
				if (osob21.x > 2.2)
					osob21.x = 2.2;
				if (osob21.x < -2.2)
					osob21.x = -2.2;
				osob21.y = osob21.y + randf(-1., 1.);
				if (osob21.y > 2.2)
					osob21.y = 2.2;
				if (osob21.y < -2.2)
					osob21.y = -2.2;
			
		}


		Population population1;
		population1.osobs.push_back(osob1);
		population1.osobs.push_back(osob2);
		population1.osobs.push_back(osob12);
		population1.osobs.push_back(osob21);
		population1.mutation = is_mutation;

		population = population1;
	}
	print_population(population, i);

}


