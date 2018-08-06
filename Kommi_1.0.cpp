#include <iostream>
#include <vector>  
#include <string>  
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <typeinfo>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  
using namespace std;
using namespace cgicc;

//�������, ����������� ����� ������ � ������
void method_Litla(vector<vector<double>> matrix, double M, vector<string> adress);

int main() {
	Cgicc formData;

	cout << "Content-type:text/html\r\n\r\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>Using GET and POST Methods</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	string str,adress;
	//����� ������� �����, ������������ � ��� double
	double  M = 9223372036854775807.0;

	form_iterator fi = formData.getElement("matrix");
	if (!fi->isEmpty() && fi != (*formData).end()) {
		//���������� ���� ������� � ���� ������ � ����������
		str = **fi;
	}
	else {
		cout << "No text entered for first name" << endl;
	}

	fi = formData.getElement("adress");
	if (!fi->isEmpty() && fi != (*formData).end()) {
		//���������� ������ ������� � ����������
		adress = **fi;
	}
	else {
		cout << "No text entered for first name" << endl;
	}

	//������ ����������� �������
	int place = str.find(",", 0);
	int kolvo = 1;
	int place1;
	while (place != -1) {
		place1 = place + 1;
		place = str.find(",", place1);
		kolvo++;
	}
	kolvo++;

	//����������� ������� 
	int len = sqrt(kolvo);
	//������� string ����������
	vector <vector<string>> matrix_distance0(len);
	//������ �������
	vector<string> mass_adress;

	//��������� ������ � ������ ��������� �� ���������
	int npos = str.find(",", 0);
	int pos = 0;
	int count = 0;
	for (int i = 0; i < len; i++) {
		while (npos != -1 && count < len) {
			matrix_distance0[i].push_back(str.substr(pos, npos - pos));
			pos = npos + 1;
			npos = str.find(",", pos);
			count++;
		}
		count = 0;

	}
	matrix_distance0[len - 1].push_back("0");//���������� ���������� ��������

	//��������� ������ � �������� �� ���������
	npos = adress.find(";", 0);
	pos = 0;
	count = 0;
	
	while (count < len) {
		mass_adress.push_back(adress.substr(pos, npos - pos));
		pos = npos + 2;
		npos = adress.find(";", pos);
		count++;
	}

	//������� ����������
	vector <vector<double>> matrix_distance(len);

	//������� �� ������� ����� � ������� �����
	double distance; string num;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			num=matrix_distance0[i][j];
			istringstream ist(num);
			ist >> distance;
			matrix_distance[i].push_back(distance);
			distance = 0;
		}
	}
	
	//��� ��������� ������ ������ � ������ ������� �� ��������� ������� ������ 
	//����� ���������� M(����� ������� �����, ������������ � ��� double)
	for (int i = 0; i < len; i++) {
		matrix_distance[i][i] = M;
	}
	
	//����� �������
	cout<<"������� ����������:"<< "<br/>\n";
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			cout << matrix_distance[i][j] << ' ';
		}
		cout << "<br/>\n";
	}
	cout << "<br/>\n";

	//��������� ����� ������ � ������
	method_Litla(matrix_distance,M,mass_adress);
	
	cout << "<br/>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}

void method_Litla(vector<vector<double>> matrix,double M,vector<string> adress) {

	//������� ���������� � ����������� �� ����� ��������
	vector <vector<double>> matrix_distance1(matrix.size()+1);
	//���.�������
	vector <vector<double>> matrix_distance2(matrix.size() + 1);
	//������� ��������
	vector<vector<int>> matrix_route(matrix.size());
	//����������� �������� � ������� �������� �� ���������� ����� ���������
	vector<int>size(matrix.size());
	//����������� �������� � ������� �������� ����� ���������� ����� ���������
	vector<int>size_new(matrix.size());

	//����� ��������
	double dist = 0, max;
	int row, col, q, w, start;
	
	//������� ����� ��������� � ����������� �� ����� ��������
	matrix_distance1[0].push_back(0);
	for (int i = 1; i < matrix.size() + 1; i++) {
		matrix_distance1[0].push_back(i);
		matrix_distance1[i].push_back(i);
	}
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix.size(); j++) {
			matrix_distance1[i + 1].push_back(matrix[i][j]);
		}
	}

	//������������� ������� ����� ���������
	matrix_distance2 = matrix_distance1;

	//������� ������ c ����������� �� ����� ��������, ����������� ������
	vector <vector<double>> matrix_grade(matrix_distance1.size());
	matrix_grade[0].push_back(0);
	for (int i = 1; i < matrix_distance1.size(); i++) {
		matrix_grade[0].push_back(i);
		matrix_grade[i].push_back(i);
	}
	for (int i = 1; i < matrix_distance1.size(); i++) {
		for (int j = 1; j < matrix_distance1.size(); j++) {
			matrix_grade[i].push_back(0);
		}
	}
	
	vector<double> di, dj;
	
	for (int st = 0; st < matrix.size(); st++) {
		di.push_back(0);
		//���� ����������� �������� �� ������� � ���������� �� � ������ di
		for (int i = 1; i < matrix_distance1.size(); i++) {
			vector<double>::iterator result = min_element(matrix_distance1[i].begin() + 1, matrix_distance1[i].end());
			di.push_back(matrix_distance1[i][distance(matrix_distance1[i].begin(), result)]);
		}

		//���������� �������� �����(������ �� ������� �������� ������ ��������������� �� ����������� ������� (di))
		for (int i = 1; i < matrix_distance1.size(); i++) {
			for (int j = 1; j < matrix_distance1.size(); j++) {
				matrix_distance1[i][j] -= di[i];
			}
		}

		//������ ����� ����������� �������� �� ������� ������� � ������� �� � ������ dj
		dj.push_back(0);
		for (int j = 1; j < matrix_distance1.size(); j++) {
			int min = 1;
			for (int i = 1; i < matrix_distance1.size(); i++) {
				if (matrix_distance1[i][j] < matrix_distance1[min][j]) {
					min = i;
				}
			}
			dj.push_back(matrix_distance1[min][j]);
		}

		//c������ �������� ��������
		for (int j = 1; j < matrix_distance1.size(); j++) {
			for (int i = 1; i < matrix_distance1.size(); i++) {
				matrix_distance1[i][j] -= dj[j];
			}
		}

		//�������� ��� ������� ��������� ������� ������
		for (int i = 1; i < matrix_distance1.size(); i++) {
			for (int j = 1; j < matrix_distance1.size(); j++) {
				if (matrix_distance1[i][j] == 0) {
					double grade = 0;
					//����� ����������� �������� �� ������� � ���� ���������, �� �������� ��� ���� �������
					vector<double> buff1, buff2;
					for (int r = 1; r < matrix_distance1.size(); r++) {
						if (r != j) {
							buff1.push_back(matrix_distance1[i][r]);
						}
					}
					vector<double>::iterator row_elem = min_element(buff1.begin(), buff1.end());
					grade += *row_elem;
					//����� ����������� �������� �� ������� � ���� ���������, �� �������� ��� ���� �������
					for (int r = 1; r < matrix_distance1.size(); r++) {
						if (r != i) {
							buff2.push_back(matrix_distance1[r][j]);
						}
					}
					vector<double>::iterator col_elem = min_element(buff2.begin(), buff2.end());
					grade += *col_elem;;
					matrix_grade[i][j] = grade;
					buff1.erase(buff1.begin(), buff1.end());
					buff2.erase(buff2.begin(), buff2.end());
				}
			}
		}

		//����� ����� ������ ������� ������ ������������ ������
		max = 0; row = 0; col = 0;
		for (int i = 1; i < matrix_grade.size(); i++) {
			vector<double>::iterator result = max_element(matrix_grade[i].begin() + 1, matrix_grade[i].end());
			if (max <= matrix_grade[i][distance(matrix_grade[i].begin(), result)]) {
				max = matrix_grade[i][distance(matrix_grade[i].begin(), result)];
				row = i; col = distance(matrix_grade[i].begin(), result);
			}
		}

		//�������� ����, ������� ������ ����� ���������� �����
		//exclusion(matrix_distance1, matrix_distance1[row][0], matrix_distance1[0][col], M);
		q = 0; w = 0;
		for (int t = 1; t < matrix_distance1.size(); t++) {
			if (matrix_distance1[0][t] == matrix_distance1[row][0]) {
				q = t;
			}
		}
		for (int j = 1; j < matrix_distance1.size(); j++) {
			if (matrix_distance1[j][0] == matrix_distance1[0][col]) {
				w = j;
			}
		}
		if (w != 0 && q != 0) {
			matrix_distance1[w][q] = M;
		}
		//�������� ������� ��������
		if (matrix_route.empty() == 1) {
			matrix_route[0].push_back(matrix_distance1[row][0]);
			matrix_route[0].push_back(matrix_distance1[0][col]);
			if (matrix_route[0].size() == matrix.size()) {
				//����� �������
				cout << "�������:" << "<br/>\n";
				for (int p = 0; p < matrix_route[0].size(); p++) {
					if (matrix_route[0][p] == 1) {
						start = p;
						break;
					}
				}
				for (int p = start; p < matrix_route[0].size(); p++) {
					cout << matrix_route[0][p] << "->";
				}
				for (int p = 0; p < start; p++) {
					cout << matrix_route[0][p] << "->";
				}
				cout<< matrix_route[0][start]<< "<br/>\n";
				cout << "<br/>\n";
				for (int p = start; p < matrix_route[0].size(); p++) {
					cout << adress[matrix_route[0][p]-1] << "->" << "<br/>\n";
				}
				for (int p = 0; p < start; p++) {
					cout << adress[matrix_route[0][p] - 1] << "->" << "<br/>\n";
				}
				cout << adress[matrix_route[0][start]-1] << "<br/>\n";
				return;
			}
		}
		else {
			for (int i = 0; i < matrix_route.size(); i++) {
				size[i] = matrix_route[i].size();
			}
			for (int i = 0; i < matrix_route.size(); i++) {
				if (matrix_route[i].empty() == 0) {
					if (matrix_route[i][matrix_route[i].size() - 1] == matrix_distance1[row][0]) {
						matrix_route[i].push_back(matrix_distance1[0][col]);
						if (matrix_route[i].size() == matrix.size()) {
							//����� �������
								cout << "�������:" << "<br/>\n";
								for (int p = 0; p < matrix_route[i].size(); p++) {
									if (matrix_route[i][p] == 1) {
										start = p;
										break;
									}
								}
								for (int p = start; p < matrix_route[i].size(); p++) {
									cout << matrix_route[i][p] << "->";
								}
								for (int p = 0; p < start; p++) {
									cout << matrix_route[i][p] << "->";
								}
								cout<< matrix_route[i][start]<< "<br/>\n";
								cout << "<br/>\n";
								for (int p = start; p < matrix_route[0].size(); p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								for (int p = 0; p < start; p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								cout << adress[matrix_route[i][start] - 1] << "<br/>\n";
								return;
							
						}
						//��������� ��������� �������
						//exclusion(matrix_distance1, matrix_route[i][matrix_route[i].size() - 1], matrix_route[i][0], M);
						q = 0; w = 0;
						for (int t = 1; t < matrix_distance1.size(); t++) {
							if (matrix_distance1[0][t] == matrix_route[i][0]) {
								q = t;
							}
						}
						for (int j = 1; j < matrix_distance1.size(); j++) {
							if (matrix_distance1[j][0] == matrix_route[i][matrix_route[i].size() - 1]) {
								w = j;
							}
						}
						if (w != 0 && q != 0) {
							matrix_distance1[w][q] = M;
						}
						break;
					}
					else {
						if (matrix_route[i][0] == matrix_distance1[0][col]) {
							matrix_route[i].insert(matrix_route[i].begin(), matrix_distance1[row][0]);
							if (matrix_route[i].size() == matrix.size()) {
								//����� �������
								cout << "�������:" << "<br/>\n";
								for (int p = 0; p < matrix_route[i].size(); p++) {
									if (matrix_route[i][p] == 1) {
										start = p;
										break;
									}
								}
								for (int p = start; p < matrix_route[i].size(); p++) {
									cout << matrix_route[i][p] << "->";
								}
								for (int p = 0; p < start; p++) {
									cout << matrix_route[i][p] << "->";
								}
								cout << matrix_route[i][start] << "<br/>\n";
								cout << "<br/>\n";
								for (int p = start; p < matrix_route[0].size(); p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								for (int p = 0; p < start; p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								cout << adress[matrix_route[i][start] - 1] << "<br/>\n";
								return;
							}
							//��������� ��������� �������
							//exclusion(matrix_distance1, matrix_route[i][matrix_route[i].size() - 1], matrix_route[i][0], M);
							q = 0; w = 0;
							for (int t = 1; t < matrix_distance1.size(); t++) {
								if (matrix_distance1[0][t] == matrix_route[i][0]) {
									q = t;
								}
							}
							for (int j = 1; j < matrix_distance1.size(); j++) {
								if (matrix_distance1[j][0] == matrix_route[i][matrix_route[i].size() - 1]) {
									w = j;
								}
							}
							if (w != 0 && q != 0) {
								matrix_distance1[w][q] = M;
							}
							break;
						}
					}
				}
			}
			for (int i = 0; i < matrix_route.size(); i++) {
				size_new[i] = matrix_route[i].size();
			}
			if (size_new == size) {
				for (int i = 0; i < matrix_route.size(); i++) {
					if (matrix_route[i].empty() == 1) {
						matrix_route[i].push_back(matrix_distance1[row][0]);
						matrix_route[i].push_back(matrix_distance1[0][col]);
						break;
					}
				}
			}
			else {
				for (int i = 0; i < matrix_route.size(); i++) {
					for (int j = i + 1; j < matrix_route.size(); j++) {
						if (matrix_route[i].empty() == 0 && matrix_route[j].empty() == 0 && matrix_route[i][0] == matrix_route[j][matrix_route[j].size() - 1]) {
							matrix_route[j].erase(matrix_route[j].end() - 1);
							for (int k = matrix_route[j].size() - 1; k > -1; k--) {
								matrix_route[i].insert(matrix_route[i].begin(), matrix_route[j][k]);
							}
							matrix_route.erase(matrix_route.begin() + j);
							matrix_route.resize(matrix_route.size() + 1);
							if (matrix_route[i].size() == matrix.size()) {
								//����� �������
								cout << "�������:" << "<br/>\n";
								for (int p = 0; p < matrix_route[i].size(); p++) {
									if (matrix_route[i][p] == 1) {
										start = p;
										break;
									}
								}
								for (int p = start; p < matrix_route[i].size(); p++) {
									cout << matrix_route[i][p] << "->";
								}
								for (int p = 0; p < start; p++) {
									cout << matrix_route[i][p] << "->";
								}
								cout << matrix_route[i][start] << "<br/>\n";
								cout << "<br/>\n";
								for (int p = start; p < matrix_route[0].size(); p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								for (int p = 0; p < start; p++) {
									cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
								}
								cout << adress[matrix_route[i][start] - 1] << "<br/>\n";
								return;
							}
							//�������� �������
							//exclusion(matrix_distance1, matrix_route[i][matrix_route[i].size() - 1], matrix_route[i][0], M);
							q = 0; w = 0;
							for (int t = 1; t < matrix_distance1.size(); t++) {
								if (matrix_distance1[0][t] == matrix_route[i][0]) {
									q = t;
								}
							}
							for (int k = 1; k < matrix_distance1.size(); k++) {
								if (matrix_distance1[k][0] == matrix_route[i][matrix_route[i].size() - 1]) {
									w = k;
								}
							}
							if (w != 0 && q != 0) {
								matrix_distance1[w][q] = M;
							}

						}
						else {
							if (matrix_route[i].empty() == 0 && matrix_route[j].empty() == 0 && matrix_route[i][matrix_route[i].size() - 1] == matrix_route[j][0]) {
								matrix_route[j].erase(matrix_route[j].begin());
								for (int k = 0; k < matrix_route[j].size(); k++) {
									matrix_route[i].push_back(matrix_route[j][k]);
								}
								matrix_route.erase(matrix_route.begin() + j);
								matrix_route.resize(matrix_route.size() + 1);
								if (matrix_route[i].size() == matrix.size()) {
									//����� �������
									cout << "�������:" << "<br/>\n";
									for (int p = 0; p < matrix_route[i].size(); p++) {
										if (matrix_route[i][p] == 1) {
											start = p;
											break;
										}
									}
									for (int p = start; p < matrix_route[i].size(); p++) {
										cout << matrix_route[i][p] << "->";
									}
									for (int p = 0; p < start; p++) {
										cout << matrix_route[i][p] << "->";
									}
									cout << matrix_route[i][start] << "<br/>\n";
									cout << "<br/>\n";
									for (int p = start; p < matrix_route[0].size(); p++) {
										cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
									}
									for (int p = 0; p < start; p++) {
										cout << adress[matrix_route[i][p] - 1] << "->" << "<br/>\n";
									}
									cout << adress[matrix_route[i][start] - 1] << "<br/>\n";
									return;
								}
								//�������� �������
								//exclusion(matrix_distance1, matrix_route[i][matrix_route[i].size() - 1], matrix_route[i][0], M);
								q = 0; w = 0;
								for (int t = 1; t < matrix_distance1.size(); t++) {
									if (matrix_distance1[0][t] == matrix_route[i][0]) {
										q = t;
									}
								}
								for (int k = 1; k < matrix_distance1.size(); k++) {
									if (matrix_distance1[k][0] == matrix_route[i][matrix_route[i].size() - 1]) {
										w = k;
									}
								}
								if (w != 0 && q != 0) {
									matrix_distance1[w][q] = M;
								}
							}
						}
					}
				}
			}

		}

		//�������� ������� - ������� ������ � �������, ��� ��������� ������� ������ � ������������ �������
		matrix_distance1.erase(matrix_distance1.begin() + row);
		for (int count = 0; count < matrix_distance1.size(); count++) {
			matrix_distance1[count].erase(matrix_distance1[count].begin() + col);
		}

		//�������� ������� ������ -  ������� ������ � �������, ��� ��������� ������������ ������
		matrix_grade.erase(matrix_grade.begin() + row);
		for (int count = 0; count < matrix_grade.size(); count++) {
			matrix_grade[count].erase(matrix_grade[count].begin() + col);
		}

		//��������� M(����� ������� �����, ������������ � double) � ��������� �������(��� ��� ����� ��������� ��� ����������� �������������, �� �� ��)
		for (int i = 1; i < matrix_distance1.size(); i++) {
			for (int j = 1; j < matrix_distance1.size(); j++) {
				if (matrix_distance1[i][0] == matrix_distance1[0][j]) {
					matrix_distance1[i][j] = M;
				}
			}
		}
		

		//��������� ������� ������
		for (int i = 1; i < matrix_grade.size(); i++) {
			for (int j = 1; j < matrix_grade.size(); j++) {
				matrix_grade[i][j] = 0;
			}
		}

		//�������� ���� ����������� ��������� �� di, dj
		di.erase(di.begin(), di.end());
		dj.erase(dj.begin(), dj.end());
	}

}