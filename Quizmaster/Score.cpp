#include "Score.h"


Score::Score(int m_score, int m_questionCount, int m_time, std::string m_userName, std::string m_gameMode)
{
	questionCount = m_questionCount;
	userName = m_userName;
	gameMode = m_gameMode;
	score = m_score;
	time = m_time;
}

Score::~Score()
{

}