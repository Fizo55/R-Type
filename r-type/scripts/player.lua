function key_z()
    local dt = (clock:get_last_tick())
    local temp = self:get_pos("y") - math.floor(dt * 0.3)

    if (temp > 0) then
        self:move(self:get_pos("x"), temp)
    end
end

function key_q()
    local dt = (clock:get_last_tick())
    local temp = self:get_pos("x") - math.floor(dt * 0.3)

    if (temp > 0) then
        self:move(temp, self:get_pos("y"))
    end
end

function key_s()
    local dt = (clock:get_last_tick())
    local temp = self:get_pos("y") + math.floor(dt * 0.3)

    if (temp < (600 - self:get_size("y"))) then
        self:move(self:get_pos("x"), temp)
    end
end

function key_d()
    local dt = (clock:get_last_tick())
    local temp = self:get_pos("x") + math.floor(dt * 0.3)

    if (temp < (800 - self:get_size("x"))) then
        self:move(temp, self:get_pos("y"))
    end
end

function key_space()
    local temp = object()
    print("ok")
    print(temp)
    print(temp:get_pos("x"))
end
