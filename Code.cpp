// 11. Сгенерировать массив из 300 строк, содержащих случайные 200000 цифр.
// Посчитать число вхождений четных и нечетных цифр в каждой строке, используя N потоков.
// Измерить время программы для N = 1, 2, 4, 10. Измерить работы время программы в каждом случае. Юзаем OMP

#include <iostream>
#include <array>
#include <string>
#include <chrono>
#include <omp.h>
#include <iomanip>

// Bad practice 2, no access restriction, doesnt matter =]
std::array<std::string, 300> custom_array;
std::array<int, custom_array.size()> even_array;

// Function, which counts evens
void counter() {
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < custom_array.size(); i++) {
			int even = 0, x = 0;
			std::string s = custom_array.at(i);
			for (auto c : s) {
				x = c - '0';
				if (x % 2 == 0) {
					even++;
				}
			}
			even_array[i] = even;
			even = 0;
		}
	}
}

int main()
{
	// Variables
	int string_lenght = 2000000;
	srand((unsigned int)time(0));
	omp_set_num_threads(12);

	// Filling Array. Also need parallelization for better performance -_-
	float progress = 0,
		increment = 100.0 / custom_array.size();
	std::cout << "Start of filling array\n";
	auto begin = std::chrono::high_resolution_clock::now();

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < custom_array.size(); i++) {
				for (int j = 0; j < string_lenght; j++) {
					custom_array.at(i) += std::to_string((rand() % 10));
				}

			// Dont want to use critical section - for better performance. In this case it affect only at appearance
			progress += increment;
			std::cout << std::left << std::setprecision(3) << std::setw(6) << progress << "%\n";
		}

	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - begin;
	std::cout << "Finish of filling array for: " << (float)duration.count() << " sec\n\n";

	omp_set_num_threads(1);
	float start1 = omp_get_wtime();
	counter();
	float end1 = omp_get_wtime();
	std::cout << "\nThreads runtime: " << end1 - start1 << " \n";
	
	omp_set_num_threads(2);
	start1 = omp_get_wtime();
	counter();
	end1 = omp_get_wtime();
	std::cout << "\nThreads runtime: " << end1 - start1 << " \n";

	omp_set_num_threads(4);
	start1 = omp_get_wtime();
	counter();
	end1 = omp_get_wtime();
	std::cout << "\nThreads runtime: " << end1 - start1 << " \n";

	omp_set_num_threads(10);
	start1 = omp_get_wtime();
	counter();
	end1 = omp_get_wtime();
	std::cout << "\nThreads runtime: " << end1 - start1 << " \n";

	for (int i = 0; i < even_array.size(); i++) {
		std::cout << std::left << "String: " << std::setw(8) << i << " Even: " << std::setw(7) << even_array.at(i) << " Odd: " << std::setw(7) << string_lenght - even_array.at(i) << "\n";
	}

	system("pause");
	return EXIT_SUCCESS;
}