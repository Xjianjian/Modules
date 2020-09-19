
#ifndef COMPLEXDRV_HARDWAREDOGMANAGER_H_
#define COMPLEXDRV_HARDWAREDOGMANAGER_H_

#define m_hd_firstFeed		0
#define m_hd_NormalFeed		1


#define m_hd_normalFeedCycle	1024
#define m_hd_firstFeedGap	(m_hd_normalFeedCycle * 3/5)


extern void hardWareDogManager(void);
#endif /* COMPLEXDRV_HARDWAREDOGMANAGER_H_ */
