#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

enum timeframe_scope { daily = 1, weekly, monthly };

struct stock_data
{
	char date[11];
	double open, high, low, close;
};

struct chart_axes
{
	char start_date[11];
	char end_date[11];
	double maximum, minimum, scale_step;
};

void draw_header()
{
	cout << endl << setw(40) << " " << "STOCK MARKET" << endl << endl;
}

void display_welcome()
{
	char c;
	draw_header();
	cout << setw(30) << " " << "Group 2" << endl
		<< setw(30) << " " << "Press 'g' to continue to the program. Press 'q' to exit." << endl;
	do {
		cin >> c;
	} while (c != 'q' && c != 'g');

	if (c == 'q')
		exit(0);
}

void enter_filenames(char* input_filename, char* output_filename)
{
	cout << "\033[2J\033[1;1H";
	draw_header();
	cout << setw(30) << " " << "Enter input filename:" << endl;
	cin >> input_filename;

	cout << "\033[2J\033[1;1H";
	draw_header();
	cout << setw(30) << " " << "Enter output filename:" << endl;
	cin >> output_filename;
}

void enter_height(int* height)
{
	cout << "\033[2J\033[1;1H";
	draw_header();
	cout << setw(30) << " "
		<< "Enter chart height (minimum 10):" << endl;
	do {
		cin >> *height;
	} while (*height < 10);
}

void enter_date_range(int* range_start, int* range_end, int total_size)
{
	cout << "\033[2J\033[1;1H";
	draw_header();

	cout << setw(30) << " " << "Enter start and end index of the range:" << endl;
	cin >> *range_start >> *range_end;

	while (*range_end > total_size || *range_start > *range_end || *range_start < 1)
	{
		if (*range_start > *range_end || *range_start < 1)
		{
			cout << "\033[2J\033[1;1H";
			cout << endl << endl << setw(30) << " "
				<< "Invalid range entered! Please enter again:" << endl;
			cin >> *range_start >> *range_end;
		}
		else if (*range_end > total_size)
		{
			cout << "\033[2J\033[1;1H";
			cout << endl << endl << setw(30) << " "
				<< "End of range exceeds available candles count! Please enter again." << endl << setw(30) << " "
				<< "Current available candles count is " << total_size << endl;
			cin >> *range_start >> *range_end;
		}
	}
}

void enter_timeframe(timeframe_scope* timeframe)
{
	int choice;

	cout << "\033[2J\033[1;1H";
	draw_header();
	cout << setw(30) << " " << "Enter timeframe for a single candle (1, 2, 3):"
		<< endl << setw(30) << " " << "1. Daily"
		<< endl << setw(30) << " " << "2. Weekly"
		<< endl << setw(30) << " " << "3. Monthly" << endl;
	do {
		cin >> choice;
	} while (choice < 1 || choice > 3);

	switch (choice)
	{
	case 1:
		*timeframe = daily;
		break;
	case 2:
		*timeframe = weekly;
		break;
	case 3:
		*timeframe = monthly;
		break;
	default:
		break;
	}
}

void parse_csv_line(char* line, stock_data* day)
{
	char* token_context = NULL;

	char* token = strtok_s(line, ",", &token_context);
	snprintf(day->date, sizeof(day->date), "%s", token);

	token = strtok_s(NULL, ",", &token_context);
	day->open = atof(token);

	token = strtok_s(NULL, ",", &token_context);
	day->high = atof(token);

	token = strtok_s(NULL, ",", &token_context);
	day->low = atof(token);

	token = strtok_s(NULL, ",", &token_context);
	day->close = atof(token);
}

stock_data* fetch_data(char* filename, int* item_count)
{
	ifstream file;
	char line[100];

	file.open(filename);
	if (!file.good())
	{
		cout << "Error loading file";
		exit(1);
	}
	file.getline(line, 100);
	while (!file.eof())
	{
		file.getline(line, 100);

		if (strlen(line) != 0) {
			++*item_count;
		}
	}

	stock_data* stock_records = new stock_data[*item_count];

	if (file.fail()) {
		file.clear();
	}

	file.seekg(0);
	*item_count = 0;

	file.getline(line, 100);
	while (!file.eof())
	{
		file.getline(line, 100);

		if (strlen(line) != 0)
		{
			parse_csv_line(line, &stock_records[*item_count]);
			++*item_count;
		}
	}
	if (file.fail()) {
		file.clear();
	}
	file.close();
	return stock_records;
}

chart_axes calculate_scale(stock_data* stock_records, int total_size, int height)
{
	chart_axes axes_data{};

	snprintf(axes_data.start_date, sizeof(axes_data.start_date), "%s", stock_records[0].date);
	snprintf(axes_data.end_date, sizeof(axes_data.end_date), "%s", stock_records[total_size - 1].date);
	axes_data.maximum = 0;
	axes_data.minimum = 999999;

	for (int i = 0; i < total_size; i++)
	{
		if (stock_records[i].high > axes_data.maximum)
			axes_data.maximum = stock_records[i].high;

		if (stock_records[i].low < axes_data.minimum)
			axes_data.minimum = stock_records[i].low;
	}
	axes_data.scale_step = (axes_data.maximum - axes_data.minimum) / height;

	return axes_data;
}

bool is_bullish(stock_data day)
{
	if (day.close > day.open)
		return true;
	else
		return false;
}

bool is_in_range(int index, int range_start, int range_end)
{
	if ((index + 1) >= range_start && (index + 1) <= range_end)
		return true;
	else
		return false;
}

char determine_symbol(stock_data day_data, double y_max, double y_min)
{
	char symbol = ' ';
	float high = day_data.high,
		low = day_data.low,
		open = day_data.open,
		close = day_data.close;

	if (high >= y_min && low < y_max)
	{
		symbol = '|';

		if (is_bullish(day_data) && close >= y_min && open < y_max)
			symbol = 'O';

		else if (!is_bullish(day_data) && open >= y_min && close < y_max)
			symbol = '#';
	}
	return symbol;
}

stock_data* merge_candles(stock_data* stock_records, int total_size, timeframe_scope timeframe, int* merged_size)
{
	int divisor;

	if (timeframe == weekly)
		divisor = 5;
	else
		divisor = 20;

	*merged_size = total_size / divisor;

	stock_data* stock_merged = new stock_data[*merged_size];

	for (int i = 0; i < *merged_size; i++)
	{
		snprintf(stock_merged[i].date, sizeof(stock_merged[i].date),
			"%s", stock_records[i * divisor + divisor - 1].date);

		stock_merged[i].open = stock_records[i * divisor].open;
		stock_merged[i].close = stock_records[i * divisor + divisor - 1].close;
		stock_merged[i].high = 0;
		stock_merged[i].low = 999999;

		for (int j = 0; j < divisor; j++)
		{
			if (stock_records[i * divisor + j].high > stock_merged[i].high)
				stock_merged[i].high = stock_records[i * divisor + j].high;

			if (stock_records[i * divisor + j].low < stock_merged[i].low)
				stock_merged[i].low = stock_records[i * divisor + j].low;
		}
	}
	return stock_merged;
}

void save_chart_to_file(stock_data* stock_records, chart_axes axes_data, char* output_filename,
	int total_size, int height, int range_start, int range_end)
{
	double y_max = axes_data.maximum, y_min = axes_data.maximum - axes_data.scale_step;

	ofstream chart_file;

	chart_file.open(output_filename);

	for (int row = 0; row < height; row++)
	{
		chart_file << setw(11) << setprecision(8) << y_max << "  ";

		for (int col = 0; col <= (total_size - 1); col++)
		{
			if (!is_in_range(col, range_start, range_end))
				continue;

			chart_file << determine_symbol(stock_records[col], y_max, y_min);
		}
		chart_file << endl;

		y_max = y_min;
		y_min -= axes_data.scale_step;
	}
	int candle_count = range_end - range_start + 1;

	chart_file << setw(11) << axes_data.minimum << " " << stock_records[range_start - 1].date;

	if (candle_count > 20)
	{
		for (int i = 1; i < (candle_count - 20); i++)
			chart_file << " ";
	}
	chart_file << " " << stock_records[range_end - 1].date;

	chart_file.close();
}

void draw_chart_on_console(stock_data* stock_records, chart_axes axes_data,
	int total_size, int height, int range_start, int range_end)
{
	double y_max = axes_data.maximum, y_min = axes_data.maximum - axes_data.scale_step;
	int printed_candles = 0;

	cout << "\033[2J\033[1;1H";

	for (int row = 0; row < height; row++)
	{
		cout << setw(11) << setprecision(8) << y_max << "  ";

		for (int col = 0; (col <= (total_size - 1) && printed_candles <= 100); col++)
		{
			if (!is_in_range(col, range_start, range_end))
				continue;

			cout << determine_symbol(stock_records[col], y_max, y_min);
			++printed_candles;
		}
		cout << endl;
		printed_candles = 0;
		y_max = y_min;
		y_min -= axes_data.scale_step;
	}
	printed_candles = range_end - range_start + 1;

	cout << setw(11) << axes_data.minimum << " " << stock_records[range_start - 1].date;

	if (printed_candles > 20)
	{
		for (int i = 1; i < (printed_candles - 20) && i <= 80; i++)
			cout << " ";
	}
	cout << " " << stock_records[range_end - 1].date;
}

int main()
{
	char choice;
	char input_filename[50] = "intc_us_data.csv";
	char output_filename[50] = "chart.txt";
	int height = 100,
		total_size = 0, merged_size = 0,
		range_start = 1, range_end = 10;
	timeframe_scope timeframe = daily;

	display_welcome();

	while (true)
	{
		enter_filenames(input_filename, output_filename);

		stock_data* stock_records = fetch_data(input_filename, &total_size);

		while (true)
		{
			enter_height(&height);

			enter_timeframe(&timeframe);

			if (timeframe == daily)
			{
				chart_axes axes_data = calculate_scale(stock_records, total_size, height);
				enter_date_range(&range_start, &range_end, total_size);

				save_chart_to_file(stock_records, axes_data, output_filename,
					total_size, height, range_start, range_end);
				draw_chart_on_console(stock_records, axes_data,
					total_size, height, range_start, range_end);
			}
			else
			{
				stock_data* stock_merged = merge_candles(stock_records, total_size, timeframe, &merged_size);
				chart_axes axes_data = calculate_scale(stock_merged, merged_size, height);
				enter_date_range(&range_start, &range_end, merged_size);

				save_chart_to_file(stock_merged, axes_data, output_filename,
					merged_size, height, range_start, range_end);
				draw_chart_on_console(stock_merged, axes_data,
					merged_size, height, range_start, range_end);

				delete[] stock_merged;
			}
			cout << endl << endl << setw(30) << " " << "The chart has been saved to file "
				<< output_filename << endl << setw(30) << " "
				<< "Press 'z' to change settings, 'g' to load new data, or 'q' to exit" << endl;
			do {
				cin >> choice;
			} while (choice != 'z' && choice != 'g' && choice != 'q');

			if (choice == 'q')
			{
				delete[] stock_records;
				exit(0);
			}
			if (choice == 'g')
			{
				delete[] stock_records;
				break;
			}
		}
	}
	return 0;
}
