#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include "resource.h"

#define _debug(x) \
std::cout << __FUNCTION__ << " : " << __LINE__ << " | "; \
std::cout  << " | (" << #x << " = " << x << ")\n"; 

HMODULE GCM() {
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModule
	);
	return hModule;
}

namespace gm {
	
	class GameMusic {
		private:
			std::unordered_map<int, sf::SoundBuffer> Sounds;
			sf::Music Melody;
			sf::Sound sound;
			
			
			std::pair<char*, DWORD> FindInMemory(int lpName, int lpType) {
				HRSRC hRes = FindResource(GCM(), MAKEINTRESOURCE(lpName), MAKEINTRESOURCE(lpType));
				HGLOBAL hData = LoadResource(GCM(), hRes);
				DWORD hSize = SizeofResource(GCM(), hRes);
				char* hFinal = (char*)LockResource(hData);
				return {hFinal, hSize};
			}

		public:

			GameMusic() {}
			~GameMusic() {}

			void SetMusic(const int lpName, const int lpType) {
				char* hFinal;
				DWORD hSize;
				std::pair<char*&, DWORD>(hFinal, hSize) = FindInMemory(lpName, lpType);
				
				Melody.openFromMemory(hFinal, hSize);
			}
			
			
			void AddSound(const int lpName, const int lpType) throw() {
				char* hFinal;
				DWORD hSize;
				std::pair<char*&, DWORD>(hFinal, hSize) = FindInMemory(lpName, lpType);
				sf::SoundBuffer SB;
				if (!SB.loadFromMemory(hFinal, hSize)) throw std::runtime_error("���� �� ������� �������.\n");
				Sounds[lpName] = SB;
			}

			void playSound(const int lpName) {
				sound.setBuffer(Sounds[lpName]);
				sound.play();
			}

			void playMusic(bool loop) { Melody.setLoop(loop);  Melody.play(); }

	};





}