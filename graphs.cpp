#include <vector>
#include <array>
#include <iostream>
#include "simulationResults.h"
#include "gnuplot-iostream.h"
#include "graphs.h"

void makeAvgGraphs(simulationResults &stats)
{
	std::cout << "Making graph of mean stat values over time...\n";
	Gnuplot gp;
	std::vector< std::vector<std::array<double, 10>>> manySimAvg{ stats.getManySimAvg() };

	std::vector<std::vector<double>> simPop, simAvgSize, simAvgSpeed, simAvgSense;
	std::vector<double> maxSize, maxSpeed, maxSense;
	std::vector<double> minSize, minSpeed, minSense;
	double maxPop{ 0 }, maxSizeVal{ 0 }, maxSpeedVal{ 0 }, maxSenseVal{ 0 };

	for (auto sim : manySimAvg)
	{
		std::vector<double> avgSize, avgSpeed, avgSense;
		std::vector<double> pop;

		for (std::array<double, 10> day : sim)
		{
			pop.push_back(day[0]);
			avgSize.push_back(day[1]);
			avgSpeed.push_back(day[2]);
			avgSense.push_back(day[3]);
		}
		simPop.push_back(pop);
		simAvgSize.push_back(avgSize);
		simAvgSpeed.push_back(avgSpeed);
		simAvgSense.push_back(avgSense);

		if (getMax(pop) > maxPop)
		{
			maxPop = getMax(pop);
		}

		if (getMax(avgSize) > maxSizeVal)
		{
			maxSizeVal = getMax(avgSize);
		}
		if (getMax(avgSpeed) > maxSpeedVal)
		{
			maxSpeedVal = getMax(avgSpeed);
		}
		if (getMax(avgSense) > maxSenseVal)
		{
			maxSenseVal = getMax(avgSense);
		}
		if (getMax(avgSense) > maxSenseVal)
		{
			maxSenseVal = getMax(avgSense);
		}
	}
	if (manySimAvg.size() == 1)
	{
		for (std::array<double, 10> day : manySimAvg[0])
		{
			maxSize.push_back(day[4]);
			maxSpeed.push_back(day[5]);
			maxSense.push_back(day[6]);
			minSize.push_back(day[7]);
			minSpeed.push_back(day[8]);
			minSense.push_back(day[9]);
		}
		maxSizeVal = getMax(maxSize);
		maxSpeedVal = getMax(maxSpeed);
		maxSenseVal = getMax(maxSense);
	}

	gp << "set terminal pngcairo crop size 1920,1080 enhanced font 'Verdana, 10'\n";
	if (manySimAvg.size() == 1)
	{
		gp << "set output 'AverageChange.png'\n";
	}
	else
	{
		gp << "set output 'AverageChange_" << manySimAvg.size() << "runs.png'\n";
		//gp << "set multiplot title '" << manySimAvg.size() << " Runs'\n";
	}

	gp << "set multiplot layout 2, 2 rowsfirst\n";

	//style commands
	gp << "set border linewidth 1.0\n";
	gp << "set style line 1 lt 1 lc rgb '#77ac30'\n"; //green
	gp << "set style line 2 lt 1 lc rgb '#0072bd'\n"; // blue
	gp << "set style line 3 lt 1 lc rgb '#d95319'\n"; // orange
	gp << "set style line 4 lt 1 lc rgb '#a2142f'\n"; // red
	gp << "set style line 5 lt 1 lc rgb '#7e2f8e'\n"; // purple"
	gp << "unset key\n";
	gp << "set xlabel 'Days'\n";

	// Population plot
	gp << "set ylabel 'Blob Population'\n";
	gp << "set yrange[0:" << (maxPop + (10)) << "]\n";
	gp << "plot";
	for (auto element : simPop)
	{
		gp << gp.file1d(element) << "with lines ls 1 lw 3";
		gp << ", ";
	}
	gp << "\n";

	if (manySimAvg.size() == 1)
	{
		gp << "set key\n";
	}

	// AvgSize plot
	gp << "set ylabel 'Blob Size'\n";
	gp << "set yrange[0:" << (maxSizeVal + 1) << "]\n";
	gp << "plot ";
	if (manySimAvg.size() == 1)
	{
		gp << gp.file1d(maxSize) << "with filledcurves x1 lt 1 lw 3 lc rgb '#950072bd' t 'Max and Min', ";
		gp << gp.file1d(minSize) << "with filledcurves x1 lt 1 lw 3 lc rgb '#FFFFFF' t ' ', ";
		gp << gp.file1d(simAvgSize[0]) << "with lines ls 2 lw 3 t 'Mean'";
	}
	else
	{
		for (auto element : simAvgSize)
		{
			gp << gp.file1d(element) << "with lines ls 2 lw 3";
			gp << ", ";
		}
	}
	gp << "\n";

	// AvgSpeed plot
	gp << "set ylabel 'Blob Speed'\n";
	gp << "set yrange[0:" << (maxSpeedVal + 1) << "]\n";
	gp << "plot ";
	if (manySimAvg.size() == 1)
	{
		gp << gp.file1d(maxSpeed) << "with filledcurves x1 lt 1 lw 3 lc rgb '#95d95319' t 'Max and Min', ";
		gp << gp.file1d(minSpeed) << "with filledcurves x1 lt 1 lw 3 lc rgb '#FFFFFF' t ' ', ";
		gp << gp.file1d(simAvgSpeed[0]) << "with lines ls 3 lw 3 t 'Mean'";
	}
	else
	{
		for (auto element : simAvgSpeed)
		{
			gp << gp.file1d(element) << "with lines ls 3 lw 3";
			gp << ", ";
		}
	}
	gp << "\n";

	// AvgSense plot
	gp << "set ylabel 'Blob Sense'\n";
	gp << "set yrange[0:" << (maxSenseVal + 1) << "]\n";
	gp << "plot ";
	if (manySimAvg.size() == 1)
	{
		gp << gp.file1d(maxSense) << "with filledcurves x1 lt 1 lw 3 lc rgb '#957e2f8e' t 'Max and Min', ";
		gp << gp.file1d(minSense) << "with filledcurves x1 lt 1 lw 3 lc rgb '#FFFFFF' t ' ', ";
		gp << gp.file1d(simAvgSense[0]) << "with lines ls 5 lw 3 t 'Mean'";
	}
	else
	{
		for (auto element : simAvgSense)
		{
			gp << gp.file1d(element) << "with lines ls 5 lw 3";
			gp << ", ";
		}
	}
	gp << "\n";

	gp << "unset multiplot\n";
}

//plot Histogram animation for sims in the range firstSim to lastSim
void makeHistogram(simulationResults &stats, int firstSim, int lastSim)
{
	assert(lastSim >= firstSim && "Value of firstSim is greater than lastSim");

	std::cout << "Making animated histogram of each stats population distribution...\n";

	//Get maximum values for x ranges and y range
	double maxPop = 0;
	double maxSize = 0;
	double maxSpeed = 0;
	double maxSense = 0;
	std::vector<std::vector<std::vector<std::vector<double>>>> manySimEach{ stats.getManySimEach() };

	for (int sim{ firstSim }; sim <= lastSim; ++sim)
	{
		if (lastSim < firstSim)
		{
			std::cout << "Value of lastSim is less than firstSim, no valid run for histogram to be created.\n";
			break;
		}

		for (auto day : manySimEach[sim])
		{
			double tempMaxSize{ getMax(day[1]) };
			double tempMaxSpeed{ getMax(day[2]) };
			double tempMaxSense{ getMax(day[3]) };
			if (day[0][0] > maxPop)
			{
				maxPop = day[0][0];
			}
			if (tempMaxSize > maxSize)
			{
				maxSize = tempMaxSize;
			}
			if (tempMaxSpeed > maxSpeed)
			{
				maxSpeed = tempMaxSpeed;
			}
			if (tempMaxSense > maxSense)
			{
				maxSense = tempMaxSense;
			}
		}

		Gnuplot gp;

		//graph style and formatting
		gp << "set yrange[0:" << maxPop << "]\n";
		gp << "set grid ytics lt 0 lw 1 lc rgb '#bbbbbb'\n"; //grey
		gp << "set grid xtics lt 0 lw 1 lc rgb '#bbbbbb'\n"; //grey
		gp << "unset key\n";

		//histogram prep
		gp << "binwidth = 0.5\n";
		gp << "binstart = -0.25\n";
		gp << "set boxwidth 0.5\n";
		gp << "set style fill solid 0.5\n";
		//magic function from gnuplotting.com that sorts data into bins and then plots
		gp << "hist = 'u (binwidth*(floor(($1-binstart)/binwidth)+0.5)+binstart):(1.0) smooth freq w boxes'\n";

		//Set output as gif
		int delay = 5000 / static_cast<int>(manySimEach[sim].size());
		gp << "set terminal gif animate delay " << delay << " crop size 1280,600 enhanced font 'Verdana, 10'\n";
		gp << "set output 'StatDistribution_Sim" << sim << ".gif'\n";
		gp << "set xlabel font 'Verdana,12'\n";
		gp << "set ylabel font 'Verdana,12'\n";

		int n = 0;
		for (std::vector<std::vector<double>> day : manySimEach[sim])
		{
			gp << "set title 'Day " << n << "' font 'Verdana,12'\n";
			++n;

			//set multiplot
			gp << "set multiplot layout 1, 3\n";

			//Size Distribution
			gp << "set xlabel 'Size'\n";
			gp << "set ylabel 'Count'\n";
			gp << "set xrange[0:" << maxSize << "]\n";
			gp << "plot '-' @hist ls 1 lc rgb '#0072bd'\n"; // blue
			gp.send1d(day[1]);

			//Speed Distribution
			gp << "set xlabel 'Speed'\n";
			gp << "set ylabel 'Count'\n";
			gp << "set xrange[0:" << maxSpeed << "]\n";
			gp << "plot '-' @hist ls 1 lc rgb '#d95319'\n"; // orange
			gp.send1d(day[2]);

			//Sense Distribution
			gp << "set xlabel 'Sense\n";
			gp << "set ylabel 'Count'\n";
			gp << "set xrange[0:" << maxSense << "]\n";
			gp << "plot '-' @hist ls 1 lc rgb '#7e2f8e'\n"; // purple"
			gp.send1d(day[3]);

			gp << "unset multiplot\n";
		}
	}
}