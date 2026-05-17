#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ---------------------------------------------
//  ANSI Colour helpers
// ---------------------------------------------
#define RST   "\033[0m"
#define BOLD  "\033[1m"
// Foreground colours
#define RED   "\033[91m"
#define YEL   "\033[93m"
#define GRN   "\033[92m"
#define CYN   "\033[96m"
#define BLU   "\033[94m"
#define MAG   "\033[95m"
#define WHT   "\033[97m"
// Background + bright text combos for banners
#define BG_RED  "\033[41m"
#define BG_BLU  "\033[44m"
#define BG_GRN  "\033[42m"
#define BG_YEL  "\033[43m"

// ---------------------------------------------
//  Base class (as provided on Classroom)
// ---------------------------------------------
class Game
{
public:
    Game(string t) { title = t; }
    virtual void play() = 0;          // pure virtual -> must be overridden
    string getTitle() const { return title; }
    virtual ~Game() = default;
private:
    string title;
};

// ---------------------------------------------
//  Monster Tamer internals (unchanged logic)
// ---------------------------------------------
enum class Type { FIRE, WATER, GRASS };
string typeName(Type t){
    if(t==Type::FIRE)  return RED  BOLD "Fire"  RST;
    if(t==Type::WATER) return BLU  BOLD "Water" RST;
    return             GRN  BOLD "Grass" RST;
}

class Monster {
protected:
    string name; Type type; int hp,maxHp,atk,def,lvl,exp;
public:
    Monster(string n,Type t,int h,int a,int d)
        :name(n),type(t),hp(h),maxHp(h),atk(a),def(d),lvl(1),exp(0){}
    virtual ~Monster()=default;
    virtual string special()=0;
    virtual int    spDmg()  =0;
    virtual int    attack() { return atk+(rand()%8-3); }
    string getName()  const { return name; }
    Type   getType()  const { return type; }
    int    getHp()    const { return hp; }
    int    getMaxHp() const { return maxHp; }
    int    getLvl()   const { return lvl; }
    bool   alive()    const { return hp>0; }
    void hit(int dmg){ hp=max(0,hp-(max(0,dmg-def/2))); }
    void heal()      { hp=maxHp; }
    void gainExp(int e){
        exp+=e;
        if(exp>=lvl*50){
            exp=0; lvl++; maxHp+=10; atk+=3; def+=2; hp=maxHp;
            cout<<"\n  "<<BG_YEL<<BOLD<<WHT<<" ** LEVEL UP! "<<name<<" is now Lv."<<lvl<<" ** "<<RST<<"\n";
            cout<<"  "<<YEL<<BOLD<<"  HP+10  ATK+3  DEF+2  (fully healed!)"<<RST<<"\n";
        }
    }
    friend ostream& operator<<(ostream& os,const Monster& m){
        int pct=20*m.hp/m.maxHp;
        string bar="[";
        for(int i=0;i<20;i++) bar+=(i<pct?"#":".");
        bar+="]";
        return os<<m.name<<" Lv."<<m.lvl<<" ["<<typeName(m.type)<<"] HP:"
                 <<m.hp<<"/"<<m.maxHp<<" "<<bar;
    }
    void save(ofstream& f) const {
        f<<name<<" "<<(int)type<<" "<<hp<<" "<<maxHp<<" "<<atk<<" "<<def<<" "<<lvl<<" "<<exp<<"\n";
    }
    void load(ifstream& f){ int t; f>>name>>t>>hp>>maxHp>>atk>>def>>lvl>>exp; type=(Type)t; }
};

class FireMon : public Monster {
public:
    FireMon(string n):Monster(n,Type::FIRE,90,20,10){}
    string special() override { return "Inferno Blast"; }
    int    spDmg()   override { return atk*2+15; }
    int    attack()  override { return atk+(rand()%15-5); }
};
class WaterMon : public Monster {
public:
    WaterMon(string n):Monster(n,Type::WATER,110,15,15){}
    string special() override { return "Tidal Wave"; }
    int    spDmg()   override { return atk*2+10; }
};
class GrassMon : public Monster {
public:
    GrassMon(string n):Monster(n,Type::GRASS,100,17,13){}
    string special() override { return "Solar Beam"; }
    int    spDmg()   override { return atk*2+12; }
};

template<typename T>
float typeBonus(T a,T d){
    if(a==Type::FIRE &&d==Type::GRASS)  return 2.0f;
    if(a==Type::WATER&&d==Type::FIRE)   return 2.0f;
    if(a==Type::GRASS&&d==Type::WATER)  return 2.0f;
    if(a==Type::FIRE &&d==Type::WATER)  return 0.5f;
    if(a==Type::WATER&&d==Type::GRASS)  return 0.5f;
    if(a==Type::GRASS&&d==Type::FIRE)   return 0.5f;
    return 1.0f;
}

int pickLocation(){
    cout<<"\n  "<<BG_GRN<<BOLD<<WHT<<" >> Choose a Location "<<RST<<"\n";
    cout<<"  "<<RED <<"[1] Volcano Cave   "<<RST<<"- Fire monsters common\n";
    cout<<"  "<<BLU <<"[2] Ocean Shore    "<<RST<<"- Water monsters common\n";
    cout<<"  "<<GRN <<"[3] Jungle Forest  "<<RST<<"- Grass monsters common\n";
    cout<<"  "<<MAG <<"[4] Random Wilds   "<<RST<<"- Anything goes!\n";
    cout<<"  > ";
    int loc;
    while(true){ cin>>loc; if(loc>=1&&loc<=4) break; cout<<"  Pick 1-4: "; }
    if(loc==1)      cout<<"  >> You enter the scorching Volcano Cave...\n";
    else if(loc==2) cout<<"  >> You walk along the misty Ocean Shore...\n";
    else if(loc==3) cout<<"  >> You venture deep into the Jungle Forest...\n";
    else            cout<<"  >> You head into the Random Wilds...\n";
    return loc;
}

unique_ptr<Monster> makeWild(int location){
    int roll=rand()%100, t;
    if(location==1)      t=(roll<70)?0:(roll<85)?1:2;
    else if(location==2) t=(roll<70)?1:(roll<85)?0:2;
    else if(location==3) t=(roll<70)?2:(roll<85)?0:1;
    else                 t=rand()%3;
    if(t==0) return make_unique<FireMon>("WildFlame");
    if(t==1) return make_unique<WaterMon>("WildWave");
    return make_unique<GrassMon>("WildLeaf");
}

class Player {
    string name;
    vector<unique_ptr<Monster>> team;
    int money=500, wins=0;
public:
    Player(string n):name(n){}
    bool add(unique_ptr<Monster> m){
        if((int)team.size()<4){ cout<<"  >> "<<m->getName()<<" joined your team!\n"; team.push_back(move(m)); return true; }
        cout<<"  Team is full! (max 4)\n"; return false;
    }
    Monster* first(){ for(auto& m:team) if(m->alive()) return m.get(); return nullptr; }
    Monster* chooseFighter(){
        vector<int> aliveIdx;
        for(int i=0;i<(int)team.size();i++) if(team[i]->alive()) aliveIdx.push_back(i);
        if(aliveIdx.empty()) return nullptr;
        if(aliveIdx.size()==1){
            cout<<"  >> Sending "<<team[aliveIdx[0]]->getName()<<" into battle!\n";
            return team[aliveIdx[0]].get();
        }
        cout<<"\n  "<<BG_BLU<<BOLD<<WHT<<" >> Pick your fighter "<<RST<<"\n";
        for(int i=0;i<(int)aliveIdx.size();i++) cout<<"  ["<<i+1<<"] "<<*team[aliveIdx[i]]<<"\n";
        cout<<"  > ";
        int choice;
        while(true){ cin>>choice; if(choice>=1&&choice<=(int)aliveIdx.size()) break; cout<<"  Invalid: "; }
        Monster* chosen=team[aliveIdx[choice-1]].get();
        cout<<"  >> "<<chosen->getName()<<" steps forward!\n";
        return chosen;
    }
    void healAll(){ for(auto& m:team) m->heal(); cout<<"  All monsters healed!\n"; }
    void addWin(){ wins++; }
    int getMoney() const { return money; }
    int getWins()  const { return wins; }
    int teamSize() const { return (int)team.size(); }
    void show() const {
        cout<<"\n  "<<BG_BLU<<BOLD<<WHT<<" [TEAM] "<<name<<"'s Team | $"<<money<<" | Wins: "<<wins<<" "<<RST<<"\n";
        for(int i=0;i<(int)team.size();i++) cout<<"  "<<i+1<<". "<<*team[i]<<"\n";
    }
    void earn(int n){ money+=n; }
    void save(const string& f) const {
        ofstream o(f); o<<name<<"\n"<<money<<"\n"<<wins<<"\n"<<team.size()<<"\n";
        for(auto& m:team){ o<<(int)m->getType()<<"\n"; m->save(o); }
        cout<<"  Game saved!\n";
    }
    bool load(const string& f){
        ifstream in(f); if(!in) return false;
        int sz; in>>name>>money>>wins>>sz; in.ignore();
        for(int i=0;i<sz;i++){
            int t; in>>t; in.ignore();
            unique_ptr<Monster> m;
            if(t==0) m=make_unique<FireMon>("");
            else if(t==1) m=make_unique<WaterMon>("");
            else m=make_unique<GrassMon>("");
            m->load(in); team.push_back(move(m));
        }
        cout<<"  Loaded! Welcome back, "<<name<<"!\n"; return true;
    }
};

bool battle(Monster& p, Monster& w, Player& player){
    bool spUsed=false;
    cout<<"\n  "<<BG_RED<<BOLD<<WHT<<"  *** BATTLE! Wild "<<w.getName()<<" appeared! ***  "<<RST<<"\n";
    cout<<"  "<<RED<<"------------------------------"<<RST<<"\n";
    while(p.alive()&&w.alive()){
        cout<<"\n  YOU : "<<p<<"\n";
        cout<<"  WILD: "<<w<<"\n\n";
        cout<<"  [1] Attack\n";
        cout<<"  [2] Special ("<<p.special()<<")";
        if(spUsed) cout<<" [USED]";
        cout<<"\n  [3] Catch (better on low HP)\n";
        cout<<"  [4] Run\n  > ";
        int c; cin>>c;
        if(c==4){ cout<<"  Got away safely!\n"; return false; }
        if(c==3){
            float hpRatio=(float)w.getHp()/w.getMaxHp();
            int chance=(int)((1.0f-hpRatio)*100)+20;
            cout<<"  Threw a capture orb... ";
            if(rand()%100<chance){ cout<<"Gotcha! "<<w.getName()<<" was caught!\n"; return true; }
            cout<<w.getName()<<" broke free! ("<<chance<<"% chance)\n";
            int wd=(int)(w.attack()*typeBonus<Type>(w.getType(),p.getType()));
            cout<<"  "<<w.getName()<<" attacks for "<<wd<<" dmg!\n";
            p.hit(wd);
            if(!p.alive()){ cout<<"  "<<p.getName()<<" fainted!\n"; return false; }
            continue;
        }
        int dmg=(c==2&&!spUsed)?(spUsed=true,p.spDmg()):p.attack();
        float mult=typeBonus<Type>(p.getType(),w.getType());
        dmg=(int)(dmg*mult);
        if(mult>1.0f) cout<<"  "<<YEL<<BOLD<<"** SUPER EFFECTIVE! **"<<RST<<"\n";
        if(mult<1.0f) cout<<"  "<<BLU<<"(not very effective...)"<<RST<<"\n";
        cout<<"  "<<p.getName()<<" deals "<<dmg<<" damage!\n";
        w.hit(dmg);
        if(!w.alive()){
            int xp=30+w.getLvl()*10;
            cout<<"  "<<w.getName()<<" fainted!  +"<<xp<<" EXP  +$50\n";
            p.gainExp(xp); return false;
        }
        int wd=(int)(w.attack()*typeBonus<Type>(w.getType(),p.getType()));
        cout<<"  "<<w.getName()<<" strikes back for "<<wd<<" damage!\n";
        p.hit(wd);
        if(!p.alive()) cout<<"  "<<p.getName()<<" fainted!\n";
    }
    return false;
}

// ---------------------------------------------
//  MonsterTamerGame  -  derived from Game
// ---------------------------------------------
class MonsterTamerGame : public Game          // <- inherits Game
{
public:
    // Constructor passes the title string up to the base class
    MonsterTamerGame() : Game("Monster Tamer v2.0") {}

    // Override the pure-virtual play() - this IS the game loop
    void play() override
    {
        srand(static_cast<unsigned>(time(0)));

        cout << BG_RED<<BOLD<<YEL
             << "  +============================+  \n"
             << "  |  ** " << getTitle() << " **  |  \n"
             << "  +============================+  "
             << RST << "\n";
        cout << "[1] New Game\n[2] Load Game\n> ";

        int c; cin >> c; cin.ignore();
        Player* player = nullptr;

        if (c == 2) {
            player = new Player("");
            if (!player->load("save.txt")) {
                delete player;
                player = nullptr;
                c = 1;
            }
        }
        if (c == 1) {
            string nm;
            cout << "Enter your name: ";
            getline(cin, nm);
            player = new Player(nm);

            cout << "\n" << BG_GRN<<BOLD<<WHT<<" >> Pick your starter: "<<RST<<"\n";
            cout << RED <<"  [1] Flamox   (Fire)  "<<RST<<"- high attack\n";
            cout << BLU <<"  [2] Aquatail (Water) "<<RST<<"- high HP\n";
            cout << GRN <<"  [3] Leafang  (Grass) "<<RST<<"- balanced\n> ";
            int s; cin >> s;
            if (s == 1)      player->add(make_unique<FireMon>("Flamox"));
            else if (s == 2) player->add(make_unique<WaterMon>("Aquatail"));
            else             player->add(make_unique<GrassMon>("Leafang"));
        }

        // Main game loop
        while (true) {
            cout << "\n" << BG_YEL<<BOLD<<"\033[30m"
                 << " [MAP] WORLD MAP | $" << player->getMoney()
                 << " | Wins:" << player->getWins()
                 << " | Team:" << player->teamSize() << "/4 "
                 << RST << "\n";
            cout << CYN<<"[1]"<<RST<<" Explore  "
                 << GRN<<"[2]"<<RST<<" My Team  "
                 << YEL<<"[3]"<<RST<<" Heal  "
                 << BLU<<"[4]"<<RST<<" Save  "
                 << RED<<"[5]"<<RST<<" Quit\n> ";
            cin >> c;

            if (c == 1) {
                Monster* p = player->chooseFighter();
                if (!p) { cout << "  No healthy monsters! Choose [3] to heal first.\n"; continue; }
                int loc = pickLocation();
                auto w = makeWild(loc);
                Monster* wptr = w.get();
                bool caught = battle(*p, *wptr, *player);
                if (caught)             { player->add(move(w)); player->addWin(); }
                else if (!wptr->alive()){ player->earn(50);     player->addWin(); }
            }
            else if (c == 2) player->show();
            else if (c == 3) player->healAll();
            else if (c == 4) player->save("save.txt");
            else { cout << "See you next time!\n"; break; }
        }

        delete player;
    }
};

// ---------------------------------------------
//  Entry point - uses the base-class pointer
// ---------------------------------------------
int main()
{
    Game* g = new MonsterTamerGame();   // polymorphic pointer to base
    g->play();                          // virtual dispatch -> MonsterTamerGame::play()
    delete g;
    return 0;
}