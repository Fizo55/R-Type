local cooldown_timer = 0

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

function update()
    local dt = (clock:get_last_tick())

    if (cooldown_timer > 0) then
        cooldown_timer = cooldown_timer - dt
    end
end

function key_space()
    if (cooldown_timer > 0) then
        return
    end

    local list_coords = any_list()
    local list_sprite = any_list()
    local list_size = any_list()
    local list_script = any_list()

    local temp = object()

    temp:add_component("coords", "position")
    temp:add_component("sprite", "sprite")
    temp:add_component("size", "size")
    temp:add_component("main_script", "script")

    list_sprite:push_str("shoot0")
    list_script:push_str("bullet_script")

    list_coords:push_int(self:get_pos("x") - self:get_size("x") / 2)
    list_coords:push_int(self:get_pos("y") - self:get_size("y") / 2)

    list_size:push_int(84)
    list_size:push_int(36)

    temp:add_parameter("coords", list_coords)
    temp:add_parameter("sprite", list_sprite)
    temp:add_parameter("size", list_size)
    temp:add_parameter("main_script", list_script)

    temp:compile()

    game:add_object(temp)

    cooldown_timer = 1200
end
