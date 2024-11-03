game:test()
name = self:get_name()
print(name)

valuex = 0
valuey = 0
print("Pos: ", valuex, " ", valuey, ". That was the empty poses")

valuex = self:get_pos("x")
valuey = self:get_pos("y")

print("Pos: ", valuex, " ", valuey, ". That was the poses")

if name == "button3" then
    self:move(valuex + 100, valuey + 10)
end

valuex = self:get_pos("x")
valuey = self:get_pos("y")

print("Pos: ", valuex, " ", valuey, ". That was the poses")

game:change_scene("level1")

function update ()
    print("ok")
end