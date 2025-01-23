local cooldown_timer = 4096
local spawn_count = 0

math.randomseed(os.time())

function create_ennemy()
    local list_coords = any_list()
    local list_sprite = any_list()
    local list_size = any_list()
    local list_script = any_list()

    local temp = object()

    temp:set_name("gen_ennemy")

    temp:add_component("coords", "position")
    temp:add_component("sprite", "sprite")
    temp:add_component("size", "size")
    temp:add_component("main_script", "script")

    list_sprite:push_str("sprite_ennemy1")
    list_script:push_str("ennemy_script")

    list_coords:push_int(math.random(self:get_pos("x"), self:get_pos("x") + self:get_size("x")))
    list_coords:push_int(math.random(self:get_pos("y"), self:get_pos("y") + self:get_size("y") - 36))

    list_size:push_int(288 / 8)
    list_size:push_int(36)

    temp:add_parameter("coords", list_coords)
    temp:add_parameter("sprite", list_sprite)
    temp:add_parameter("size", list_size)
    temp:add_parameter("main_script", list_script)

    temp:compile()

    game:add_object(temp)
end

function update()
    local dt = (clock:get_last_tick())

    if (cooldown_timer > 0) then
        cooldown_timer = cooldown_timer - dt
        return
    end

    local max_time = (10000 - (spawn_count * 100))

    if (max_time < 0) then
        max_time = 1000
    end

    cooldown_timer = (math.random(0, max_time))
    spawn_count = spawn_count + 1

    create_ennemy()
end
