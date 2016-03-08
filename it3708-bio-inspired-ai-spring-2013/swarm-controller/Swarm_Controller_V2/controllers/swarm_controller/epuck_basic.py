
from controller import *   # controller comes with Webots
import time                # A Python primitive module
import math                #   "            "
import Image               # An extra Python module (that you'll have to download)
import imagepro            # A module provided by Keith Downing for this assignment

# This is the basic class for controlling an epuck robot in the Webots simulator.  In theory, the
# same code can also run a physical epuck robot with just the "flip of a switch" - although there are small
# differences.  

# The class hierarchy is Robot => DifferentialWheels => epuck_basic, where the superclass, Robot, and
# DifferentialWheels are written by the Webots people, while we defined epuck_basic.  By inheriting
# from epuck_basic, you're controller should have access to basic robot functionality such as:
# a) reading distance sensors, b) viewing camera images from the robot's "eye", c) setting wheel speeds of
# the robot
# Here is the main class of your controller.
# This class defines how to initialize and run your controller.
# Note that this class derives Robot and so inherits all its functions

# When using this code with a neural-network-based controller, you may only need to call
# a small set of the routines defined below, such as:
# run_timestep, set_wheel_speeds, get_proximities, and snapshot


class EpuckBasic (DifferentialWheels):

  max_wheel_speed = 1000
  num_dist_sensors = 8
  num_light_sensors = 8
  num_leds = 8
  encoder_resolution = 159.23 # for wheel encoders
  tempo = 0.5  # Upper velocity bound = Fraction of the robot's maximum velocity = 1000 = 1 wheel revolution/sec  
  wheel_diameter = 4.1 # centimeters
  axle_length = 5.3 # centimeters

# Final 4 slots not used in Webots but included for use with physical epucks that are not driven by Webots.
  max_spin_rate = tempo * (wheel_diameter / axle_length) 
  robot_camera_xres = 40 # Need to save X and Y resolution for the epuck camera
  robot_camera_yres = 40
  timestep_duration = 1 # Real-time seconds per timestep

# Initialization needs to setup the camera and the distance sensors.  The timestep is
# found in the Webots WORLD file associated with an epuck controller.  BE SURE TO CALL THIS or
# something similar in order to get access to camera and sensor data, along with the timestep.

  def basic_setup(self, tempo = 1.0):
      self.timestep = int(self.getBasicTimeStep()) # Fetched from WorldInfo.basicTimeStep (in the Webots world)
      self.tempo = tempo
      self.enableEncoders(self.timestep)
      self.dist_sensor_values = [0 for i in range(self.num_dist_sensors)]
      self.dist_sensors = [self.getDistanceSensor('ps'+str(x)) for x in range(self.num_dist_sensors)]  # distance sensors
      map((lambda s: s.enable(self.timestep)), self.dist_sensors) # Enable all distance sensors
      self.light_sensor_values = [0 for __ in range(self.num_light_sensors)]
      self.light_sensors = [self.getLightSensor('ls'+str(x)) for x in range(self.num_light_sensors)]  # light sensors
      map((lambda s: s.enable(self.timestep)), self.light_sensors) # Enable all light sensors
      self.leds = [self.getLED('led' + str(i)) for i in range(self.num_leds)]
      self.green_led = self.getLED('led8')
      self.front_led = self.getLED('led9')
      self.front_led.set(1)
      self.green_led_status = 0
      #self.accelerometer = self.getAccelerometer("accelerometer")
      #self.accelerometer.enable(self.timestep)

      
      #self.accelerometer_values = [0]*3
      #self.accelerometer_values = [self.getAccelerometer(1)]


  def set_LEDS(self, LED):
      for i in range(self.num_leds):
            self.leds[i].set(int(LED[i]))

# **** TIMED ACTION ***

# The routines do_timed_action and run_timestep are explicit commands to the robot to actually DO
# something (i.e. move its wheels) for a given amount of time.  The actual setting of the wheel speeds
# is done via independent calls to routines such as "move" (below).

#  This is a CRITICAL method for "Robot" objects to insure synching of actuators and sensors.
# The parameter in the call to "step" is duration in milliseconds, and it needs to be a multiple of the timestep parameter, hence
# the division by timestep, rounding, and then multiplication by timestep.  The "duration" argument to
# do_timed_action is in SECONDS (hence the multiplication by 1000 prior to rounding in computing ms_second).

  def do_timed_action(self, duration = False):
      if duration:
          ms_duration = int(round(duration*1000/self.timestep)*self.timestep)
      else: ms_duration = self.timestep

      if self.getMode() == 0: # Running the simulator
          self.step(ms_duration)
      else: # Running a real robot
          print "Doing timed robot action"
          self.step(ms_duration)
          self.stop_moving() # I seem to need this to halt the previous action
          self.step(self.timestep)
          #self.stop_moving()
          #self.wait(duration)
          #self.stop_moving()

# This gets called by ANNs when they want their associated agent to do it's timestep of activity.  The setting of
# motor values and the reading of sensors are done by other ANN code such that run_timestep just needs to
# activate the bot for a brief period (with the current motor settings in effect).

  def run_timestep(self, cycles = 1):
      for i in range(cycles):
          self.do_timed_action()

  def wait(self, seconds = 1.0):
      print "waiting"
      time.sleep(seconds) 

# This could have been named "translate", as it causes the robot to
# move forward or backward along a line, but not to turn.
# In the methods MOVE and MOVE_WHEELS, note that the call to set_wheel_speeds does NOT
# cause the robot to move.  It only loads in the wheel-speed values so that they are
# relevant the NEXT TIME the epuck is asked to run a timestep.  We force that
# running via the call to do_timed_action.

  def move(self,speed=1.0,duration =1.0, dir = 'forward'):
      print "Moving"
      s = min(1.0, abs(speed))
      if dir == 'forward':
          self.set_wheel_speeds(left=s,right=s)
      elif dir == 'backward':
          self.set_wheel_speeds(left = -s, right = -s)
      self.do_timed_action(duration)

# A version of move that takes the two wheel speeds (between -1 and 1) as basis.  This
# version permits turning, since both wheel speeds can be specified independently.

  def move_wheels(self, left = 0.0, right = 0.0, duration = 1.0):
      ls = max(-1.0, min(1.0, left)) 
      rs = max(-1.0, min(1.0, right)) 
      self.set_wheel_speeds(left = ls, right = rs)
      self.do_timed_action(duration)

# When running real robots, we occasionally need to explicitly tell the
# robot to stop.

  def stop_moving(self): self.set_wheel_speeds(0,0)

# This is the lowest-level movement method, which calls the Webot's method "setSpeed" with two
# integer values in the range [-1000, 1000] or tighter if "tempo" is smaller than 1.
# The arguments "left" and "right" are in the range [-1,1].

  def set_wheel_speeds(self,left = 0.0, right = 0.0):
      #print "Setting wheel speeds: ", "Left =", left ,"  Right = ", right
      #ms = self.tempo*self.max_wheel_speed
      left = min(left, 1000)
      right = min(right, 1000)
      ms = 1
      self.setSpeed(int(left*ms),int(right*ms))

  def timed_review(self, feedback):
      return int(((float(5) / (10 - feedback)) * 100)+20)
    
#  **** SENSORS and CAMERA ****

# This fetches the values of the proximity sensors and returns them in a list.  The sensors need to
# be initialized/fetched before this routine can be run.  This is done in the "Initialize" method near the
# top of the class definitions.

  def get_proximities(self):
      for i in range(self.num_dist_sensors):
          self.dist_sensor_values[i] = self.dist_sensors[i].getValue()
      return self.dist_sensor_values

  def get_lights(self):
      for i in range(self.num_light_sensors):
          self.light_sensor_values[i] = self.light_sensors[i].getValue()
      return self.light_sensor_values

  def green_LED_blink(self):
    if self.green_led_status == 0:
        self.green_led_status = 1
    else:
        self.green_led_status = 0
    self.green_led.set(int(self.green_led_status))




