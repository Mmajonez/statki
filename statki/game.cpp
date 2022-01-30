#include <iostream>
#include <string>
#include <vector>

using namespace std;

Game games[200];

pair <int, int> getCoord(string s) {
	int a = int(s[0]);
	int x;
	int y;
	if (a >= 97 && a <= 122) {
		x = a - 97;
	}
	else if (a >= 65 && a <= 90) {
		x = a - 65;
	}
	if (s.length() == 2) y = int(s[1]) - 1;
	if (s.length() == 3) y = (int(s[1]) * 10 + int(s[2])) - 1;

	return make_pair(x, y);
}

class Board {
private:
	int ship[10][10]; // 0 - empty 1 - ship 2 - hit ship 3 - hit empty
	int statki[10][10];
	int numShips = 11;
public:
	Board() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				ship[i][j] = 0;
			}
		}
	}

	~Board() {}

	void insertShip(int x, int y) {
		ship[x][y] = 1;
	}

	bool isShip(int x, int y) {
		if (ship[x][y] == 1) return true;
		else return false;
	}

	int getPos(int x, int y) {
		return ship[x][y];
	}

	bool isSunken(int x, int y) {
		if (statki[x + 1][y] == statki[x][y] || statki[x - 1][y] == statki[x][y] || statki[x][y + 1] == statki[x][y] || statki[x][y - 1] == statki[x][y]) {
			return false;
		}
		else{
			numShips--;
			return true;
		}
	}

	void updateStatki(int x, int y, int val) {
		statki[x][y] = val;
	}

	int getNumShips() {
		return numShips;
	}
};


class Game {
public:
	Board board1;
	Board board2;
	

	bool shoot(Board b, int x, int y) {
		if (b.getPos(x,y) == 0) {
			return '3';
		}
		else if (b.getPos(x,y) == 1) {
			if (b.isSunken(x, y) == false) {
				return '2';
			}
			else if (b.isSunken(x, y) == true && b.getNumShips() > 0) {
				return '4';
			}
			else {
				return '6';
			}
			b.updateStatki(x, y, 0);
		}
		else if (b.getPos(x, y) == 2 or b.getPos(x, y) == 3) {
			return '5';
		}
	}

	void setShips(Board b) {
		char hv;
		string coord;
		bool correct = false;
		cout << "5-segment ship" << endl;
		while (correct == true) {
			cout << "put it horizontally or vertically? (h/v)" << endl;
			cin >> hv;
			if (hv != 'v' && hv != 'h') {
				cout << "Wrong character, only v and h allowed" << endl;
				continue;
			}
			cout << "set coordinates of the first segment (A1 - J10)" << endl;
			cin >> coord;
			pair <int, int> res = getCoord(coord);
			if (hv = 'h') {
				if (res.second + 5 < 10) {
					b.insertShip(res.first, res.second);
					b.insertShip(res.first, res.second + 1);
					b.insertShip(res.first, res.second + 2);
					b.insertShip(res.first, res.second + 3);
					b.insertShip(res.first, res.second + 4);
					correct == true;
					b.updateStatki(res.first, res.second, 1);
					b.updateStatki(res.first, res.second + 1, 1);
					b.updateStatki(res.first, res.second + 2, 1);
					b.updateStatki(res.first, res.second + 3, 1);
					b.updateStatki(res.first, res.second + 4, 1);
				}
				else {
					cout << "Ship is out of the board! Try again!" << endl;
					continue;
				}
			}
			else {
				if (res.first + 5 < 10) {
					b.insertShip(res.first, res.second);
					b.insertShip(res.first + 1, res.second);
					b.insertShip(res.first + 2, res.second);
					b.insertShip(res.first + 3, res.second);
					b.insertShip(res.first + 4, res.second);
					correct == true;
					b.updateStatki(res.first, res.second, 1);
					b.updateStatki(res.first + 1, res.second, 1);
					b.updateStatki(res.first + 2, res.second, 1);
					b.updateStatki(res.first + 3, res.second, 1);
					b.updateStatki(res.first + 4, res.second, 1);
					
				}
				else {
					cout << "Ship is out of the board! Try again!" << endl;
					continue;
				}
			}
			correct = false;
			cout << "4-segment ship" << endl;
			while (correct == true) {
				cout << "put it horizontally or vertically? (h/v)" << endl;
				cin >> hv;
				if (hv != 'v' && hv != 'h') {
					cout << "Wrong character, only v and h allowed" << endl;
					continue;
				}
				cout << "set coordinates of the first segment (A1 - )" << endl;
				cin >> coord;
				pair <int, int> res = getCoord(coord);
				if (hv = 'h') {
					if (res.second + 5 < 10) {
						if (b.isShip(res.first, res.second) == false && b.isShip(res.first, res.second + 1) == false && b.isShip(res.first, res.second + 2) == false && b.isShip(res.first, res.second + 3) == false) {
							b.insertShip(res.first, res.second);
							b.insertShip(res.first, res.second + 1);
							b.insertShip(res.first, res.second + 2);
							b.insertShip(res.first, res.second + 3);
							correct == true;
							b.updateStatki(res.first, res.second, 2);
							b.updateStatki(res.first, res.second + 1, 2);
							b.updateStatki(res.first, res.second + 2, 2);
							b.updateStatki(res.first, res.second + 3, 2);

						}
						else {
							cout << "There already is a ship. Ships cannot intersect. Try again!" << endl;
						}


					}
					else {
						cout << "Ship is out of the board! Try again!" << endl;
						continue;
					}
				}
				else {
					if (res.first + 5 < 10) {
						if (b.isShip(res.first, res.second) == false && b.isShip(res.first + 1, res.second) == false && b.isShip(res.first + 2, res.second) == false && b.isShip(res.first + 3, res.second) == false) {
							b.insertShip(res.first, res.second);
							b.insertShip(res.first + 1, res.second);
							b.insertShip(res.first + 2, res.second);
							b.insertShip(res.first + 3, res.second);
							correct == true;
							b.updateStatki(res.first, res.second, 2);
							b.updateStatki(res.first + 1, res.second, 2);
							b.updateStatki(res.first + 2, res.second, 2);
							b.updateStatki(res.first + 3, res.second, 2);
						}
					}
					else {
						cout << "Ship is out of the board! Try again!" << endl;
						continue;
					}
				}
				for (int i = 1; i < 2; i++) {
					correct = false;
					cout << "3-segment ship number " << i << endl;
					while (correct == true) {
						cout << "put it horizontally or vertically? (h/v)" << endl;
						cin >> hv;
						if (hv != 'v' && hv != 'h') {
							cout << "Wrong character, only v and h allowed" << endl;
							continue;
						}
						cout << "set coordinates of the first segment (A1 - )" << endl;
						cin >> coord;
						pair <int, int> res = getCoord(coord);
						if (hv = 'h') {
							if (res.second + 5 < 10) {
								if (b.isShip(res.first, res.second) == false && b.isShip(res.first, res.second + 1) == false && b.isShip(res.first, res.second + 2) == false) {
									b.insertShip(res.first, res.second);
									b.insertShip(res.first, res.second + 1);
									b.insertShip(res.first, res.second + 2);
									correct == true;
									b.updateStatki(res.first, res.second, 3 + i);
									b.updateStatki(res.first, res.second + 1, 3 + i);
									b.updateStatki(res.first, res.second + 2, 3 + i);
								}
								else {
									cout << "There already is a ship. Ships cannot intersect. Try again!" << endl;
								}

							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
						else {
							if (res.first + 5 < 10) {
								if (b.isShip(res.first, res.second) == false && b.isShip(res.first + 1, res.second) == false && b.isShip(res.first + 2, res.second) == false) {
									b.insertShip(res.first, res.second);
									b.insertShip(res.first + 1, res.second);
									b.insertShip(res.first + 2, res.second);
									correct == true;
									b.updateStatki(res.first, res.second, 3 + i);
									b.updateStatki(res.first + 1, res.second, 3 + i);
									b.updateStatki(res.first + 2, res.second, 3 + i);
								}
							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
					}
				}
				for (int i = 1; i < 3; i++) {
					correct = false;
					cout << "2-segment ship number " << i << endl;
					while (correct == true) {
						cout << "put it horizontally or vertically? (h/v)" << endl;
						cin >> hv;
						if (hv != 'v' && hv != 'h') {
							cout << "Wrong character, only v and h allowed" << endl;
							continue;
						}
						cout << "set coordinates of the first segment (A1 - )" << endl;
						cin >> coord;
						pair <int, int> res = getCoord(coord);
						if (hv = 'h') {
							if (res.second + 5 < 10) {
								if (b.isShip(res.first, res.second) == false && b.isShip(res.first, res.second + 1) == false) {
									b.insertShip(res.first, res.second);
									b.insertShip(res.first, res.second + 1);
									correct == true;
									b.updateStatki(res.first, res.second, 5 + i);
									b.updateStatki(res.first, res.second + 1, 5 + i);
								}
								else {
									cout << "There already is a ship. Ships cannot intersect. Try again!" << endl;
								}


							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
						else {
							if (res.first + 5 < 10) {
								if (b.isShip(res.first, res.second) == false && b.isShip(res.first + 1, res.second) == false) {
									b.insertShip(res.first, res.second);
									b.insertShip(res.first + 1, res.second);
									correct == true;
									b.updateStatki(res.first, res.second, 5 + i);
									b.updateStatki(res.first + 1, res.second, 5 + i);
								}
							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
					}
				}
				for (int i = 1; i < 4; i++) {
					correct = false;
					cout << "1-segment ship number " << i << endl;
					while (correct == true) {
						cout << "put it horizontally or vertically? (h/v)" << endl;
						cin >> hv;
						if (hv != 'v' && hv != 'h') {
							cout << "Wrong character, only v and h allowed" << endl;
							continue;
						}
						cout << "set coordinates of the first segment (A1 - )" << endl;
						cin >> coord;
						pair <int, int> res = getCoord(coord);
						if (hv = 'h') {
							if (res.second + 5 < 10) {
								if (b.isShip(res.first, res.second) == false) {
									b.insertShip(res.first, res.second);
									correct == true;
									b.updateStatki(res.first, res.second, 8 + i);
								}
								else {
									cout << "There already is a ship. Ships cannot intersect. Try again!" << endl;
								}
							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
						else {
							if (res.first + 5 < 10) {
								if (b.isShip(res.first, res.second) == false) {
									b.insertShip(res.first, res.second);
									correct == true;
									b.updateStatki(res.first, res.second, 8 + i);
								}
							}
							else {
								cout << "Ship is out of the board! Try again!" << endl;
								continue;
							}
						}
					}
					cout << "All ships placed" << endl;
				}
			}
		}
	}

	void show(Board you, Board opp){
		cout << "    A   B   C   D   E   F   G   H   I   J			    A   B   C   D   E   F   G   H   I   J" << endl;
		cout << "------------------------------------------ 	         ------------------------------------------" << endl;
		for (int x = 0; x < 10; x++) {
			cout << x + 1 << "|";
			for (int y = 0; y < 10; y++) {
				if (you.getPos(x,y) == 0) {
					cout << "   |";
				}
				else if (you.getPos(x,y) == 1) {
					cout << " s |";
				}
				else if (you.getPos(x,y) == 2) {
					cout << " # |";
				}
				else if (you.getPos(x,y) == 3) {
					cout << " ~ |";
				}
			}
			if (x < 9) cout << "           " << x + 1 << "|";
			else cout << "         " << x + 1 << "|";
			for (int y = 0; y < 10; y++) {
				if (opp.getPos(x,y) == 0) {
					cout << "   |";
				}
				else if (opp.getPos(x,y) == 1) {
					cout << "   |";
				}
				else if (opp.getPos(x,y) == 2) {
					cout << " # |";
				}
				else if (opp.getPos(x,y) == 3) {
					cout << " ~ |";
				}
			}
			cout << endl;
			cout << "------------------------------------------           ------------------------------------------" << endl;
		}

	}

};

