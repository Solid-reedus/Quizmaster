
#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef SCORE
#define SCORE

class Score
{
	public:
	Score(int m_score, int m_questionCount, int m_time,	std::string m_userName, std::string m_gameMode);
	~Score();

	int score;
	int questionCount;
	int time;
	std::string userName;
	std::string gameMode;
};

#endif
