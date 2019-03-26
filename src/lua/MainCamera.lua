MainCamera = Script {
	onCreate = function(self)
		local root = self.node.root
		local map = root:findNodeByType("TMXMap")
		self.extentX = map.width/2
		self.extentY = map.height/2
		self.target = root:findNodeByName("player")
	end,
	onPostUpdate = function(self)
		local zoom = self.node.zoom
		if Input.isKeyPressed(81) then
			zoom = zoom + 0.1
		elseif Input.isKeyPressed(69) then
			zoom = zoom - 0.1
		end
		self.node.zoom = zoom

		if self.target then
			local position = self.target.position
			local x = position.x
			local y = position.y
			local halfViewportWidth = self.node.viewport.x/2 * zoom
			local halfViewportHeight = self.node.viewport.y/2 * zoom
			x = math.clamp(x, -self.extentX + halfViewportWidth, self.extentX - halfViewportWidth)
			y = math.clamp(y, -self.extentY + halfViewportHeight, self.extentY - halfViewportHeight)
			self.node.position.x = x
			self.node.position.y = y
		end
	end
}