/*
 *  LCWordPuzzleGame.h
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_WordPuzzle_Game_H__
#define __LC_WordPuzzle_Game_H__

#include "cocos2d.h"

USING_NS_CC;

class LCWordPuzzleAlphabet;
class LCWordPuzzleGame: public CCLayer
{
protected:
	CCArray *m_pLoadedSprites;
	CCArray *m_pBlockSprites;
	CCArray *m_pCharacterSprites;
	CCArray *m_pFinishedCharacterSprites;
	
	/**
	 @brief 현재 단어의 이미지에 대한 참조를 set/get
	 */
	CCSprite* m_pCurrentWordImage; 	//added by YoungJae Kwon
	
	char *m_pCurrentWord;
	int m_iIndexOfTargetCharacter;
	int m_iNumberOfFinishedCharacter;
	bool m_bIsFinished;
	
	/**
	 @brief	Contains wordIdx of word that can show(length limit).
	 */
	CC_PROPERTY(CCArray*,m_pIndexArray,IndexArray);
	
	int m_iIndex;
	
	/**
	 @brief A Process, Load Sprite and Arrange it.
	 
	 필요한 배열 및 상태값 초기화, 이미지 로딩 및 배치 함수 호출에 관련된 일을 수행한다.
	 */
	void loadSprites();
	
	/**
	 @brief A Process, Load Sprite and Arrange it.
	 
	 loadSprites의경우 인덱스 0부터 로딩 시작 불가능하므로 초기에는 이 함수를 호출할것
	 */
	void loadInitialSprites();
	
	/**
	 @brief shuffle alphabet sprites randomly.
	 @param CCArray* the array contains LCWordPuzzleAlphabet*
	 
	 알파벳들을 입력받아서 서로의 위치를 임의로 섞어준다.
	 */
	void shuffleSprites(CCArray* alphabetSpriteArray);
	
	/**
	 @brief callback functions when incorrectCharacter's animations finished.
	 */
	void didIncorrectCharacterAnimationFinished();
	
	/**
	 @brief callback functions when correctCharacter's animations finished.
	 */
	void didCorrectCharacterAnimationFinished();
	
	/**
	 @brief callback functions when correctCharacter's met block.
	 */
	void didIncorrectCharacterMetBlockAnimationFinished();
	
	/**
	 @brief load word indexes and add indexArray.
	 @param CCArray* 게임에서 사용될 단어들의 wordIdx를 저장할 수 있는 변수.
	 
	 게임에서 사용될 단어들의 wordIdx들을 순서대로 indexArray에 집어 넣는다.
	 */
	virtual void loadIndex(CCArray* wordIdxArray) = 0;
	
	/**
	 @brief return word using array and index.
	 @return char* 현재 차례의 단어 headword
	
	 indexArray와 현재 차례인 index에 해당하는 단어를 찾아 return한다.
	 */
	virtual const char* getWordWithIndex(CCArray* wordIdxArray, int index) = 0;
	
	/**
	 @brief return image using array and index.
	 @return CCSprite* 현재 차례의 단어의 이미지
	 
	 indexArray와 현재 차례인 index에 해당하는 이미지를 찾아서 return한다.
	 주의할 점 : image를 돌려주기 전에 위치정보를 미리 설정해서 돌려주어야 한다.
	 */
	virtual CCSprite* getImageWithIndex(CCArray* wordIdxArray, int index) = 0;
	
	/**
	 @brief return box image.
	 @return CCSprite* box image.
	 
	 게임의 중간쯤에 사용되는 빈 박스의 이미지를 돌려준다.
	 */
	virtual CCSprite* getBoxImage() = 0;
	
	/**
	 @brief return alphabet image.
	 @param char alphabet character like 'a', 'b'.
	 @return LCWordPuzzleAlphabet* alphabet image.
	 
	 입력된 알파벳을 이용해서 게임에서 사용되는 알파벳의 이미지를 만들어 돌려준다.
	 LCWordPuzzleAlphabet은 CCSprite를 상속받은 이미지로서 단순히 이미지만 만들어서 돌려주고 위치는 나중에 세팅한다.
	 */
	virtual LCWordPuzzleAlphabet* getAlphabetImage(char alphabet) = 0;
	
	/**
	 @brief set position of boxes and alphabets.
	 @param char* the word.
	 @param CCArray* the array of CCSprite* box.
	 @param CCArray* the array of LCWordPuzzleAlphabet* alphabet.
	 
	 박스와 알파벳의 위치를 설정한다.
	 주의할 점은 알파벳의 LCWordPuzzleAlphabet* 는 setPosition과 함께 setOriginalPosition 도 설정해주어야 한다.
	 */
	virtual void arrangeComponents(const char* word, CCArray* boxArray, CCArray* alphabetArray) = 0;
	
	// Event Handler
	//////// deprecated in 11.10.07 by LEETAEHO
	virtual void didCorrectCharacterSelected();
	virtual void didCorrectCharacterAnimated();
	virtual void didIncorrectCharacterSelected();
	virtual void didIncorrectCharacterAnimated();
	virtual void didFinishedWord();
	
	// Event Handler (현재 사용되고 있는 단어를 사용하기 위한 기능 추가)
	virtual void didCorrectCharacterSelected(CCArray* wordIdxArray, int index);
	virtual void didCorrectCharacterAnimated(CCArray* wordIdxArray, int index);
	virtual void didIncorrectCharacterSelected(CCArray* wordIdxArray, int index);
	virtual void didIncorrectCharacterMetBlock(CCArray* wordIdxArray, int index);
	virtual void didIncorrectCharacterAnimated(CCArray* wordIdxArray, int index);
	virtual void didFinishedWord(CCArray* wordIdxArray, int index);
	virtual ccTime delayTimeOfChangeWord(CCArray* wordIdxArray, int index);
	
	// added by YoungJae Kwon
	virtual void didWordImageTouched(CCSprite* touchedSprite);
	virtual void didIndexArrayReset(); // 인덱스 어레이를 바꿀때  호출
	
	// delay 액션 핸들을 위한 함수
	virtual void didDelayedChangeWord();
	
	// animation이 끝날때 해제하는 부분 (현재는 FadeOut 애니메이션)
	void didSpriteAnimated(CCNode* node);
	
	
public:
	LCWordPuzzleGame();
	virtual ~LCWordPuzzleGame();
	
	/**
	 @brief	Implement init code here.
	 @return true    Initialize success.	
	 @return false   Initialize failed.
	 */
	virtual bool init();
	
	// implement CCTouchDelegate
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
};


/**
 @brief contains alphabet, alphabetSprite, and original position.
 
 퍼즐 아래쪽의 알파벳의 정보를 담고 있는 클래스.
 지칭하고 있는 문자와 문자의 그림, 그리고 애니메이션이 끝난 후 돌아갈 위치를 저장하고 있다.
 */
class LCWordPuzzleAlphabet : public CCSprite
{
public:
	LCWordPuzzleAlphabet();
	virtual ~LCWordPuzzleAlphabet();
	
	// getters and setters
	CC_PROPERTY(char, m_cAlphabet, Alphabet);
	CC_PROPERTY(CCPoint, m_tOriginalPosition, OriginalPosition);
protected:
	
};

#endif // __LC_WordPuzzle_Game_H__