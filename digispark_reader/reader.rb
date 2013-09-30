require 'rubygems'
require 'bundler/setup'

require 'digiusb'

while (spark = DigiUSB.sparks.last).nil?
  puts "Waiting for device..."
  sleep 1
end

loop do
  print spark.gets
end

