#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SimdArithmeticLibrary.h"
#include <immintrin.h>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimdArithmeticTester
{		
	TEST_CLASS(SimdArithmeticLibrary)
	{
	public:
		
		TEST_METHOD(test_that_add_is_correct)
		{			
			std::vector<float> input1;
			std::vector<float> input2;
			std::vector<float> output;

			const int SIZE = 100;
			for (int i=0; i<SIZE;i++)
			{
				input1.push_back(i);
				input2.push_back(2*i);
			}
			add(input1, input2, output);

			for (int i=0; i<SIZE;i++)
			{
				Assert::AreEqual(3*i, (int) output[i]);
			}
		}

		TEST_METHOD(test_that_fastAdd_equals_add)
		{			
			std::vector<float> input1;
			std::vector<float> input2;
			std::vector<float> outputExpected;
			std::vector<float> outputFast;

			const int SIZE = 100;
			for (int i=0; i<SIZE;i++)
			{
				input1.push_back(i);
				input2.push_back(2*i);
			}

			//For measuring time, use:
			//::QueryPerformanceCounter
			//::QueryPerformanceFrequency
			//Read about it in:
			//		https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx

			// Measure time of normal add

			LARGE_INTEGER StartingTimeAdd, EndingTimeAdd, ElapsedMicrosecondsAdd, StartingTimeFastAdd, EndingTimeFastAdd, ElapsedMicrosecondsFastAdd;
			LARGE_INTEGER Frequency;

			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&StartingTimeAdd);


			add(input1, input2, outputExpected);

			QueryPerformanceCounter(&EndingTimeAdd);
			ElapsedMicrosecondsAdd.QuadPart = EndingTimeAdd.QuadPart - StartingTimeAdd.QuadPart;
			ElapsedMicrosecondsAdd.QuadPart *= 1000000;
			ElapsedMicrosecondsAdd.QuadPart /= Frequency.QuadPart;


			// Measure time of SIMD add

			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&StartingTimeFastAdd);

			fastAdd(input1, input2, outputFast);

			QueryPerformanceCounter(&EndingTimeFastAdd);
			ElapsedMicrosecondsFastAdd.QuadPart = EndingTimeFastAdd.QuadPart - StartingTimeFastAdd.QuadPart;
			ElapsedMicrosecondsFastAdd.QuadPart *= 1000000;
			ElapsedMicrosecondsFastAdd.QuadPart /= Frequency.QuadPart;

			int timeofRegularAdd = ElapsedMicrosecondsAdd.QuadPart;
			int timeofFastAdd = ElapsedMicrosecondsFastAdd.QuadPart;

			
			Assert::AreNotEqual(timeofRegularAdd,timeofFastAdd);

			for (int i=0; i<SIZE;i++)
			{
				Assert::AreEqual(outputExpected[i], outputFast[i]);
			}
		}
	};
}
