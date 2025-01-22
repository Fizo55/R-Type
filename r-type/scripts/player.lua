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
    local list_coords = any_list()
    local list_sprite = any_list()
    local list_size = any_list()
    local list_script = any_list()

    local temp = object()

    print("start")
    print(temp)

    temp:add_component("coords", "position")
    temp:add_component("sprite", "sprite")
    temp:add_component("size", "size")
    temp:add_component("main_script", "script")

    print("push str")

    list_sprite:push_str("shoot0")
    list_script:push_str("bullet_script")

    print("push int")

    list_coords:push_int(0)
    list_coords:push_int(0)

    print("push int")

    list_size:push_int(200)
    list_size:push_int(200)

    print("add params")

    temp:add_parameter("coords", list_coords)
    temp:add_parameter("sprite", list_sprite)
    temp:add_parameter("size", list_size)
    temp:add_parameter("main_script", list_script)

    print("compile")

    temp:compile()

    print("add obj")

    game:add_object(temp)

    print("end")
end
