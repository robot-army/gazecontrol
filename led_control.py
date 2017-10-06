#   Gaze Control - A real-time control application for Tobii Pro Glasses 2.
#
#   Copyright 2017 Shadi El Hajj
#
#   Licensed under the Apache License, Version 2.0 (the 'License');
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an 'AS IS' BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

MINWIDTH = 1
MAXWIDTH = 4
MAXDIST = 1000
LEDS_PER_BOX = 28
MAX_FIDUCIALS = 6

class LedControl():

    def map(self, x, in_min, in_max, out_min, out_max):
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    def get_led_params(self, detection_params):
        string = ''
        for i in range(0, MAX_FIDUCIALS):
            if i in detection_params:
                string += self.get_led_param_per_fiducial(detection_params[i])
            else:
                string += '0'*(LEDS_PER_BOX*3)

        return string + '\n'


    def get_led_param_per_fiducial(self, params):
        string = ''
        within_roi = params['within_roi']
        if within_roi:
            string += '030'*LEDS_PER_BOX

        else:
            distance = int(round(params['distance']))
            angle = int(round(params['angle']))
            dist = self.map(distance, 0, MAXDIST, MAXWIDTH, MINWIDTH)
            centreled = self.map(angle+45, -180, 180, 0, LEDS_PER_BOX)
            #make the ramp one-sided
            one_sided_ramp = [3*(i/float(dist)) for i in range(0, dist)]
            # make the ramp two sided
            two_sided_ramp = one_sided_ramp + list(reversed(one_sided_ramp))
            padded_ramp = two_sided_ramp + [0]*(LEDS_PER_BOX-len(two_sided_ramp))

            #centre the ramp around the right pixel and set the pixel colour
            for i in range(0, LEDS_PER_BOX):
                index = (centreled-i+int(dist)) % (LEDS_PER_BOX)
                val = padded_ramp[index]
                string += '0' + str(int(val)) + str(int(val))            

        return string
