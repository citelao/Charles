#!/usr/bin/env ruby

ix = 2
iy = 0

nx = 2**0.5/2
ny = 2**0.5/2

m = ix*nx + iy*ny

a = nx**2 + ny**2
b = -2*m*ny
c = m**2-nx**2

ans = (-b + (b**2 - 4*a*c)**0.5) / (2*a)
other = (-b - (b**2 - 4*a*c)**0.5) / (2*a)

class Reflect
	attr_accessor :i, :n

	def go
		m = (-@i * @n) * @n
		f = i + 2 * @n

		return Vector.new(f.x, f.y)
	end
end

class Vector
	attr_accessor :x, :y

	def initialize(x, y)
		@x = x
		@y = y
	end

	def ==(p)
		@x == p.x && @y == p.y
	end

	def magnitude
		(@x**2 + @y**2)**0.5
	end

	def *(p)
		return @x * p.x + @y * p.y if p.is_a? Vector

		Vector.new(@x * p, @y * p)
	end

	def +(p)
		Vector.new(@x + p.x, @y + p.y)
	end

	def coerce(obj)
		case obj
		when Numeric 
			return self,obj
		end
	end

	def -@
		Vector.new(-@x, -@y)
	end
end

describe Reflect do
	before(:each) do
		@ref = Reflect.new
		@ref.i = Vector.new(0, -1)
		@ref.n = Vector.new(0, 1)
	end

	it "should set i" do
		@ref.i.should == Vector.new(0, -1)
	end

	it "should set n" do
		@ref.n.should == Vector.new(0, 1)
	end

	it "should have the same magnitude" do
		@ref.go.magnitude.should == @ref.i.magnitude
	end

	it "should have opposite dot products" do
		(@ref.i * @ref.n).should == -(@ref.go * @ref.n)
	end

	it "should flip variable for straight normal" do
		@ref.go.should == (Vector.new(0, 1))
	end

	it "should sideways" do		
		@ref.i = Vector.new(1,1)
		@ref.n = Vector.new(-1,-1)
		@ref.go.should == (Vector.new(-1,-1))
	end 

	it "should more sideways" do
		@ref.i = Vector.new(1, -1)
		@ref.n = Vector.new(0, 1)
		@ref.go.should == (Vector.new(1, 1))
	end

	it "should glance" do
		@ref.i = Vector.new(1, 0)
		@ref.n = Vector.new(-1, 0)
		@ref.go.should == (Vector.new(-1, 0))
	end
end