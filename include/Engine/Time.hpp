#pragma once

/**
 * Class for managing time-related functionalities
 */
class Time
{
private:
    friend class Application;

    /**
     * Delta time
     */
    float m_deltaTime;

    /**
     * Time scale
     */
    float m_timeScale;

    /**
     * @brief Constructor
     */
    Time();

public:

    /* Delete copy constructor */
    Time(const Time&) = delete;

    /* Delete assignment operator */
    void operator=(const Time&) = delete;

    /**
     * Destructor
     */
    ~Time();

    /**
     * Gets the singleton instance for this class
     * @return Singleton instance for this class
     */
    static Time& GetInstance();

    /**
     * @brief Gets the current time scale.
     *
     * The time scale defines how fast time passes in the game.
     * For example, if time scale is 2.0, time passes twice as fast.
     * If time scale is 1.0, time passes normally, and if time scale is 0.5,
     * time passes twice as slow.
     * @return Time scale
     */
    static float GetTimeScale();

    /**
     * @brief Sets the current time scale
     * @param[in] New time scale
     */
    static void SetTimeScale(const float& timeScale);

    /**
     * @brief Gets the time elapsed since the previous frame.
     * @return Time elapsed since the previous frame
     */
    static float GetDeltaTime();

};

