#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <array>
using std::string,std::cout,std::endl,std::vector,std::cin,std::array;
class Player {
private:
	int capital = 10000;//капитал
	int fabrika = 2;//количество фабрик
	int autofabrika = 0;//количество автоматических фабрик
	int ESM = 4;
	int EGP = 2;
	bool bankrot = false;
	static int counter;//количество игроков
	array<int,2> zayavkaESM;//заявка на ЕМС
	array<int,2> offerEGP;//предложение по ЕГП
public:
	Player()
	{
		counter++;
		Id = counter;
	}
	string name;
	int Id;
	array<int,2> Get_zayavkaESM() { return zayavkaESM; }
	//void Set_zayavkaESM(int zayavka[2]) { zayavka[0] = Get_zayavkaESM()[0]; zayavka[1] = Get_zayavkaESM()[1]; }
	array<int,2> Get_offerEGP() { return offerEGP; }
	int GetCapital() const{ return capital; }
	int GetFabrika() const{ return fabrika; }
	int GetAutoFabrika() const{ return autofabrika; }
	int GetESM() const{ return ESM; }
	int GetEGP()const { return EGP; }
	bool GetBankrot() const{ return bankrot; }
	int GetCounter() { return counter; }
	void SeniorPlayer(int numberPlayers)//(количество игроков)
	{
		Id--;
		if (Id == 0) Id = numberPlayers;
	}
	bool StatusBankrot()
	{
		if (capital > 0)
			return false;
		else return true;
	}
	void ZayavkaESM()//заявка на ЕМС
	{
		cout << "Введи количество ЕМС которое хочешь купить и за какую стоимость" << endl;
		cin >> zayavkaESM[0] >> zayavkaESM[1];
	}
	void OfferEGP()//предложение ЕГП
	{
		cout << "Введи количество ЕГП которое хочешь продать и за какую стоимость" << endl;
		cin >> offerEGP[0] >> offerEGP[1];
	}
	void ESMinEGP()//переработка ЕСМ в ЕГП
	{
		int work = fabrika + autofabrika * 2;//количество товара перерабатываемого за один раз
		cout << "Сколько сырья хотите переработать?" << endl;
		int SM;//количество сырья для переработки
		int expense;//стоимость производства
		while (true)
		{
			cin >> SM;
			if (SM > ESM)
				cout << "У вас нет столько сырья выберите не больше "<<ESM<<" единиц" << endl;
			else if (SM > autofabrika * 2 + fabrika)
				cout << "У вас недостаточно фабрик для обработки выбранного сырья, выберите не больше " << autofabrika * 2 + fabrika << " единиц сырья" << endl;
			else break;
		}
		
		if (autofabrika > 0) {
			int optionF;
			while (true)
			{
				cout << "На каких фабриках запустим производство?\nНа обычных фабриках - 1\nНа автоматизированых - 2" << endl;
				cin >> optionF;
				if (optionF == 2) {
					if (SM % 2 == 0) {
						if (SM / 2 <= autofabrika)
							expense = SM / 2;
						else expense = (SM - autofabrika * 2) * 2000 + autofabrika * 3;
					}
					else
						if (SM == 1)
							expense = 2000;
						else if ((SM - 1) / 2 <= autofabrika)
							expense = SM / 2 * 3000 + 2000;
						else expense = (SM - autofabrika * 2) * 2000 + autofabrika * 3000;
					break;
				}
				else if (optionF == 1) {
					if (SM > fabrika)
						cout << "Увас не достаточно обычных фабрик для переработки такого количества сырья выберите заново" << endl;
					else if (optionF != 1 && optionF != 2)
						cout << "Некорректный ввод" << endl;
					else { expense = SM * 2000; break; }
				}
			}
		}
		capital -= expense;
		ESM -= SM;
		EGP += SM;
		if (StatusBankrot())
		{
			cout << "Вы банкрот!!!\nДля вас игра окончена" << endl;
			capital = 0; fabrika = 0; autofabrika = 0; ESM = 0; EGP = 0;
		}
	}
	void Loan();//запрос ссуды
	int RepaymentLoan();//выплата по ссуде
	void ErectFabrika();//строительство фабрик
	int Nalog();//расчет налогов
};
int Player::counter = 0;
class Bank {
private:
	Bank(){}
	static Bank* ptr_bank;
	int motion=0;//номер хода
	vector<int> players;//количество игроков вошедших в игру
	int aliveplayers=0;//количество активных игроков
	int level=3;//уровень цен
	int minESM;//мин стоимость ЕСМ
	int maxEGP;//макс стоимость ЕГП
	int countESM=0, countEGP=0;//предложение по ЕСМ и ЕГП
	//таблица уровня цен(коэфициент ЕСМ, мин цена ЕСМ, коэфициент ЕГП, макс цена ЕГП)
	double Value[5][4]{ {1,800,3,6500},{1.5,650,2.5,6000},{2,500,2,5500},{2.5,400,1.5,5000},{3,300,1,4500} };
	// матрица вероятности перехода уровня
	int Mass[5][12]{ {1,1,1,1,2,2,2,2,3,3,4,5,},{1,1,1,2,2,2,2,3,3,3,4,5},{1,2,2,2,3,3,3,3,4,4,4,5},
		{1,2,3,3,3,4,4,4,4,5,5,5},{1,2,3,3,4,4,4,4,5,5,5,5} };
	int Level(int Mass[5][12]) const{
		int j = rand() % 12;
		return Mass[this->level-1][j];
	}
public:
	Bank(Bank&) = delete;
	~Bank() { ptr_bank = nullptr; }
	static Bank* get_ptr() {
		if (ptr_bank == nullptr)
		{
			ptr_bank = new Bank();
		}
		return ptr_bank;
	}
	void Players(int Id) { players.push_back(Id); }//метод увеличивающий количество игроков
	void Aliveplayers(bool bankrot)//посчет игроков не банкротов
	{
		for (int i = 0; i < players.size(); i++)
			if (!bankrot)
				aliveplayers++;
	}
	int NextMotion() { return ++motion; }//следующий ход
	void NextLevel() { level = Level(Mass); }//следующий уровень цен
	void minESMmaxEGP() { minESM = Value[level-1][1]; maxEGP = Value[level-1][3]; }
	void ZayavkaESM(vector<Player> zayavka){
		
	}
	
};
Bank* Bank::ptr_bank = nullptr;
int main()
{			
	system("chcp 1251>NUL");
	srand(time(NULL));
	Bank* bank = Bank::get_ptr();
	vector<Player> Players;
	vector<array<int, 2>> ZayavkaESM;
	vector<array<int, 2>> OfferEGP;
	bool add = false;
	/*do
	{
		add = false;
		char YN;
		cout << "Добавить игрока?\ny - добавить n - не добавлять" << endl;
		cin >> YN;
		if (YN == 'Y' || YN == 'y') {
			Player Play;
			Players.push_back(Play);
			add = true;
		}
	} while (add);
	for (auto game : Players)
	{
		game.ZayavkaESM();
		ZayavkaESM.push_back(game.Get_zayavkaESM());
		game.OfferEGP();
		OfferEGP.push_back(game.Get_offerEGP());
	}*/
	Player pl;
	pl.ESMinEGP();
	
}
