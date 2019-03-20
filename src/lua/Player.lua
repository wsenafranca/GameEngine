Player = Script {
	onCreate = function(self)
		self.rot = 0
		self.node.width = 100
		self.node.height = 100
		print(self.node.properties)
		self.node.properties.test = "test"
		print(self.node.properties.test:asString())
	end,
	onUpdate = function(self, dt)
		if Input.isKeyPressed(68) then
			self.node.position.x = self.node.position.x + 1000*dt
		elseif Input.isKeyPressed(65) then
			self.node.position.x = self.node.position.x - 1000*dt
		end

		self.rot = self.rot + 45*dt
		self.node.rotate = math.rad(self.rot)
	end
}