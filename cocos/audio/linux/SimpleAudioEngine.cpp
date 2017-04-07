/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <iostream>

#include "audio/include/SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"

namespace CocosDenshion
{

struct SimpleAudioEngineLinux {
    SimpleAudioEngine * engine = nullptr;
    int musicid;
    float effectsvolume;
    std::string musicpath;
};

SimpleAudioEngineLinux * g_SimpleAudioEngineLinux = nullptr;

SimpleAudioEngine* SimpleAudioEngine::getInstance()
{
    if (!g_SimpleAudioEngineLinux) {
        g_SimpleAudioEngineLinux = new SimpleAudioEngineLinux();
        g_SimpleAudioEngineLinux->engine = new SimpleAudioEngine();
    }
    return g_SimpleAudioEngineLinux->engine;
}

void SimpleAudioEngine::end()
{
    if (g_SimpleAudioEngineLinux) {
        delete g_SimpleAudioEngineLinux->engine;
        delete g_SimpleAudioEngineLinux;
    }
    g_SimpleAudioEngineLinux = nullptr;
}

SimpleAudioEngine::SimpleAudioEngine()
{
    g_SimpleAudioEngineLinux->musicid = -1;
    g_SimpleAudioEngineLinux->effectsvolume = 1.0f;
}

SimpleAudioEngine::~SimpleAudioEngine()
{
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* filePath)
{
    g_SimpleAudioEngineLinux->musicpath = filePath;
    cocos2d::experimental::AudioEngine::preload(filePath);
}

void SimpleAudioEngine::playBackgroundMusic(const char* filePath, bool loop)
{
    g_SimpleAudioEngineLinux->musicpath = filePath;
    g_SimpleAudioEngineLinux->musicid =
        cocos2d::experimental::AudioEngine::play2d(filePath, loop);
}

void SimpleAudioEngine::stopBackgroundMusic(bool releaseData)
{
    cocos2d::experimental::AudioEngine::stop(g_SimpleAudioEngineLinux->musicid);
    if (releaseData) {
        cocos2d::experimental::AudioEngine::uncache
            (g_SimpleAudioEngineLinux->musicpath.c_str());
    }
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    cocos2d::experimental::AudioEngine::pause
        (g_SimpleAudioEngineLinux->musicid);
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    cocos2d::experimental::AudioEngine::resume
        (g_SimpleAudioEngineLinux->musicid);
}

void SimpleAudioEngine::rewindBackgroundMusic()
{
    cocos2d::experimental::AudioEngine::setCurrentTime
        (g_SimpleAudioEngineLinux->musicid, 0);
}

bool SimpleAudioEngine::willPlayBackgroundMusic()
{
    return g_SimpleAudioEngineLinux->musicid != -1;
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return cocos2d::experimental::AudioEngine::getState
        (g_SimpleAudioEngineLinux->musicid)
        == cocos2d::experimental::AudioEngine::AudioState::PLAYING;
}

//
// properties
//

/**
 * The volume of the background music within the range of 0.0 as the minimum and 1.0 as the maximum.
 * @js getMusicVolume
 * @lua getMusicVolume
 */
float SimpleAudioEngine::getBackgroundMusicVolume()
{
    return cocos2d::experimental::AudioEngine::getVolume
        (g_SimpleAudioEngineLinux->musicid);
}

/**
 * Set the volume of background music.
 *
 * @param volume must be within the range of 0.0 as the minimum and 1.0 as the maximum.
 * @js setMusicVolume
 * @lua setMusicVolume
 */
void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    cocos2d::experimental::AudioEngine::setVolume
        (g_SimpleAudioEngineLinux->musicid, volume);
}

/**
 * The volume of the effects within the range of 0.0 as the minimum and 1.0 as the maximum.
 */
float SimpleAudioEngine::getEffectsVolume()
{
    return g_SimpleAudioEngineLinux->effectsvolume;
}

/**
 * Set the volume of sound effects.
 *
 * @param volume must be within the range of 0.0 as the minimum and 1.0 as the maximum.
 */
void SimpleAudioEngine::setEffectsVolume(float volume)
{
    g_SimpleAudioEngineLinux->effectsvolume = volume;
}

/**
 * Play sound effect with a file path, pitch, pan and gain.
 *
 * @param filePath The path of the effect file.
 * @param loop Determines whether to loop the effect playing or not. The default value is false.
 * @param pitch Frequency, normal value is 1.0. Will also change effect play time.
 * @param pan   Stereo effect, in the range of [-1..1] where -1 enables only left channel.
 * @param gain  Volume, in the range of [0..1]. The normal value is 1.
 * @return The sound id.
 *
 * @note Full support is under development, now there are limitations:
 *     - no pitch effect on Samsung Galaxy S2 with OpenSL backend enabled;
 *     - no pitch/pan/gain on win32.
 */
unsigned int SimpleAudioEngine::playEffect(const char* filePath, bool loop, float pitch, float pan, float gain)
{
    return cocos2d::experimental::AudioEngine::play2d
        (filePath, loop, gain * g_SimpleAudioEngineLinux->effectsvolume);
}

/**
 * Pause playing sound effect.
 *
 * @param soundId The return value of function playEffect.
 */
void SimpleAudioEngine::pauseEffect(unsigned int soundId)
{
    cocos2d::experimental::AudioEngine::pause(soundId);
}

/**
 * Pause all playing sound effect.
 */
void SimpleAudioEngine::pauseAllEffects()
{
    cocos2d::experimental::AudioEngine::pauseAll();
}

/**
 * Resume playing sound effect.
 *
 * @param soundId The return value of function playEffect.
 */
void SimpleAudioEngine::resumeEffect(unsigned int soundId)
{
    cocos2d::experimental::AudioEngine::resume(soundId);
}

/**
 * Resume all playing sound effect.
 */
void SimpleAudioEngine::resumeAllEffects()
{
    cocos2d::experimental::AudioEngine::resumeAll();
}

/**
 * Stop playing sound effect.
 *
 * @param soundId The return value of function playEffect.
 */
void SimpleAudioEngine::stopEffect(unsigned int soundId)
{
    cocos2d::experimental::AudioEngine::stop(soundId);
}

/**
 * Stop all playing sound effects.
 */
void SimpleAudioEngine::stopAllEffects()
{
    cocos2d::experimental::AudioEngine::stopAll();
}

/**
 * Preload a compressed audio file.
 *
 * The compressed audio will be decoded to wave, then written into an internal buffer in SimpleAudioEngine.
 *
 * @param filePath The path of the effect file.
 * @js NA
 */
void SimpleAudioEngine::preloadEffect(const char* filePath)
{
    cocos2d::experimental::AudioEngine::preload(filePath);
}

/**
 * Unload the preloaded effect from internal buffer.
 *
 * @param filePath The path of the effect file.
 */
void SimpleAudioEngine::unloadEffect(const char* filePath)
{
    cocos2d::experimental::AudioEngine::uncache(filePath);
}

} // namespace CocosDenshion
