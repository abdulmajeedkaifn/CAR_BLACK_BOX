# CAR_BLACK_BOX

**Description:**

The Car Black Box is an embedded system designed to monitor and log real-time events and vehicle data. Acting as an onboard diagnostic dashboard, it tracks critical metrics such as time, vehicle speed, and event occurrences while enabling users to interact with the system through a key-based interface.

**Features**

1] Default Screen
  Displays real-time information:
    Current time.
    Vehicle speed.
    Latest event.
    
2] Login and Navigation
  Key-based navigation for interacting with system menus.
  Transitions to the Default Screen after 3 seconds of inactivity on login.
  
3] Main Menu
i. View Log:
    Browse logged events using UP/DOWN keys.
    Displays details:
      Event Number.
      Event Signature.
      Event Time.
      Speed at the Event.
    Rollover navigation on reaching the end of the log.
    System continues capturing events in real-time while viewing logs.
    Long press UP Key to return to Main Menu.
    
ii. Set Time:
    Modify system time using key-based inputs.
    Blinking Seconds Field indicates the current editable field.
    UP Key: Increment the time value.
    DOWN Key: Switch between fields.
    Rollover values upon reaching maximum limits.
    Long press UP Key to return to Main Menu.
    
4] Event Capture
Captures and stores critical events in real-time regardless of the system mode.
Event log format:
  Event Signature.
  Event Time.
  Vehicle Speed at the Event.
  
**Technologies Used**

  Embedded C programming.
  Microcontroller peripherals (Timers, Interrupts, GPIO).
  Real-time event logging mechanisms.
  
**How It Works**

1] Initialization:
System initializes with the Default Screen.

2] User Interaction:
UP/DOWN keys enable navigation through menus.
Long key presses trigger specific actions (e.g., accessing Main Menu, logging out).

3] Event Logging:
Events are captured in real-time and logged in memory with specific attributes.
Logs are accessible via the View Log menu.

4] Time Adjustment:
Users can adjust the system time using the Set Time menu.

**Future Improvements**

Integrating additional event triggers for comprehensive monitoring.
Expanding memory for increased log storage capacity.
Adding wireless communication for remote event retrieval.
