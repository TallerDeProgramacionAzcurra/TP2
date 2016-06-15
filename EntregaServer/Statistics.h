/*
 * StageStatistics.h
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

class Statistics
{
public:
	Statistics(): m_enemiesKilled(0),
				  m_shoots(0),
				  m_hits(0),
				  m_points(0)
	{
	}

	void reset()
	{
		m_enemiesKilled = 0;
		m_shoots = 0;
		m_hits = 0;
		m_points = 0;
	}

	int getEnemiesKilled() { return m_enemiesKilled; }
	int getShoots() { return m_shoots; }
	int getHits() { return m_hits; }
	float getAccuracy() { return ((float)m_shoots/ (float)m_hits); }
	int getPoints() { return m_points; }

	void incrementEnemiesKilled (int amount) { m_enemiesKilled += amount; }
	void incrementShoots (int amount) { m_shoots += amount; }
	void incrementHits(int amount) { m_enemiesKilled += m_hits; }
	void incrementPoints (int amount) { m_enemiesKilled += m_points; }

private:
	int m_enemiesKilled;
	int m_shoots;
	int m_hits;
	int m_points;
};



#endif /* STATISTICS_H_ */
