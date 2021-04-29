#include "Engine/Time.hpp"

/**
 * @brief Constructor
 */
Time::Time()
    : m_deltaTime(0.0f)
    , m_timeScale(1.0f)
{
}

/**
 * Destructor
 */
Time::~Time()
{
}

/**
 * Gets the singleton instance for this class
 */
Time& Time::GetInstance()
{
    static Time instance;
    return instance;
}

/**
 * @brief Gets the current time scale.
 *
 * The time scale defines how fast time passes in the game.
 * For example, if time scale is 2.0, time passes twice as fast.
 * If time scale is 1.0, time passes normally, and if time scale is 0.5,
 * time passes twice as slow.
 * @return Time scale
 */
float Time::GetTimeScale()
{
    return GetInstance().m_timeScale;
}

/**
 * @brief Sets the current time scale
 * @param[in] New time scale
 */
void Time::SetTimeScale(const float& timeScale)
{
    GetInstance().m_timeScale = timeScale;
}

/**
 * @brief Gets the time elapsed since the previous frame.
 * @return Time elapsed since the previous frame
 */
float Time::GetDeltaTime()
{
    Time& instance = GetInstance();
    return instance.m_deltaTime * instance.m_timeScale;
}

