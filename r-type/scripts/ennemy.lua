local cooldown_timer = 480

function create_bullet()
    local list_coords = any_list()
    local list_sprite = any_list()
    local list_size = any_list()
    local list_script = any_list()

    local temp = object()

    temp:add_component("coords", "position")
    temp:add_component("sprite", "sprite")
    temp:add_component("size", "size")
    temp:add_component("main_script", "script")

    list_sprite:push_str("shoot_ennemy0")
    list_script:push_str("ennemy_bullet_script")

    list_coords:push_int(self:get_pos("x") + self:get_size("x") / 2 - ((216 / 3) / 2))
    list_coords:push_int(self:get_pos("y") + self:get_size("y") / 2 - 18 / 2)

    list_size:push_int(216 / 3)
    list_size:push_int(18)

    temp:add_parameter("coords", list_coords)
    temp:add_parameter("sprite", list_sprite)
    temp:add_parameter("size", list_size)
    temp:add_parameter("main_script", list_script)

    temp:compile()

    game:add_object(temp)
end

function update()
    local dt = (clock:get_last_tick())

    self:move(self:get_pos("x") - math.floor(dt * 0.4), self:get_pos("y"))

    if (self:get_pos("x") < -self:get_size("x")) then
        game:remove_object(self)
        self = nil
    end

    if (cooldown_timer > 0) then
        cooldown_timer = cooldown_timer - dt
        return
    end

    create_bullet()

    cooldown_timer = math.random(0, 2800)
end
