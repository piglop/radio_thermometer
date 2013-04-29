require "serialport"
 
#params for serial port
port_str = "/dev/ttyACM0"  #may be different for you
baud_rate = 115200
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE
 
sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)

buffer = ""
begin
  loop do
    line = sp.gets
    buffer << line if line
    if buffer =~ /\r\n/
      line, buffer = buffer.split(/\r\n/, 2)
      p line
      codes = line.split.map { |n| n.to_i(16) }
      if codes.size == 11
        p codes.map { |x| x.to_s(16) }
        low = codes[-2]
        high = codes[-1]
        temp = ((high << 8) | low) / 16.0
        p temp
      end
    end
      
  end
ensure
    sp.close
end
