#!/usr/bin/env ruby -w


#
# format
# -
# name  # comment
#

file = nil
state = nil
#ARGF.binmode
#ARGF.set_encoding("BINARY")
ARGF.each { |line|

	line.chomp!

	case state
	when nil
		if line == '-'
			state = :name
		end

	when :name
		if line.match(/^([A-Za-z0-F]+)\s?#?/)
			state = :data
			filename = $1
			file = File::new("Help.Files/#{filename}", "w")
			#file.set_encoding("BINARY")
		end

	when :data
		if line == '-'
			state = :name
			file = nil
		else
			file.puts(line)
		end


		
	end

}
