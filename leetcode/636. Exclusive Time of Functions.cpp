class Solution {
public:
	struct Line {
		int id;
		bool start;
		int time;
		Line(int id, bool start, int time) : id(id), start(start), time(time) {};
	};

	Line getLine(string s) {
		int colon1 = s.find(":", 0);
		int colon2 = s.find(":", colon1 + 1);
		string fid = s.substr(0, colon1);
		string start = s.substr(colon1 + 1, colon2 - (colon1 + 1));
		string time = s.substr(colon2 + 1);
		return Line(stoi(fid), start == "start", stoi(time));
	}

	vector<int> exclusiveTime(int n, vector<string>& logs) {
		vector<int> T(n, 0);
		deque<int> id_stack;

		for (int i = 1; i < logs.size(); i++)
		{
			Line log1 = getLine(logs[i - 1]);
			Line log2 = getLine(logs[i]);

			if (log1.start)	id_stack.push_back(log1.id);
			else id_stack.pop_back();

			int d = log2.time - log1.time;
			if (log1.start)
				T[log1.id] += log2.start ? d : d + 1;
			else
			{
				if (log2.start)
					T[id_stack.back()] += d - 1;
				else
					T[log2.id] += d;
			}
		}

		return T;
	}
};
