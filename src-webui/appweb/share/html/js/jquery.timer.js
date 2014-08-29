/**
 * jQuery Timer Plugin (jquery.timer.js)
 * @version 1.0.1
 * @author James Brooks <jbrooksuk@me.com>
 * @website http://james.brooks.so
 * @license MIT - http://jbrooksuk.mit-license.org
 */

(function($) {
	jQuery.timer = function(interval, callback, options) {
		// Create options for the default reset value
		var options = jQuery.extend({ reset: 500 }, options);
		var interval = interval || options.reset;

		if(!callback) { return false; }

		var Timer = function(interval, callback, disabled) {
			// Only used by internal code to call the callback
			this.internalCallback = function() { callback(self); };

			// Clears any timers
			this.stop = function() { 
				if(this.state === 1 && this.id) {
					clearInterval(self.id); 
					this.state = 0;
					return true;
				}
				return false;
			};
			// Resets timers to a new time
			this.reset = function(time) {
				if(self.id) { clearInterval(self.id); }
				var time = time || options.reset;
				this.id = setInterval($.proxy(this.internalCallback, this), time);
				this.state = 1;
				return true;
			};
			// Pause a timer.
			this.pause = function() {
				if(self.id && this.state === 1) {
					clearInterval(this.id);
					this.state = 2;
					return true;
				}
				return false;
			};
			// Resumes a paused timer.
			this.resume = function() {
				if(this.state === 2) {
					this.state = 1;
					this.id = setInterval($.proxy(this.internalCallback, this), this.interval);
					return true;
				}
				return false;
			};

			// Set the interval time again
			this.interval = interval;
			
			// Set the timer, if enabled
			if (!disabled) {
				this.id = setInterval($.proxy(this.internalCallback, this), this.interval);
				this.state = 1;
			}

			var self = this;
		};

		// Create a new timer object
		return new Timer(interval, callback, options.disabled);
	};
})(jQuery);
