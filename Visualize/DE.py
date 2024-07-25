from manim import *

class DE_Mutation(Scene):
	def construct(self):
		#MObject Declaration
		ax = Axes(x_range=[0,10],y_range=[0,10],tips=True)

		s1 = VGroup(MathTex('\\Vec{S}_{1}'),Dot(ax.c2p(3,5))).set_color(RED)
		s2 = VGroup(MathTex('\\Vec{S}_{2}'),Dot(ax.c2p(6,7))).set_color(GREEN)
		s3 = VGroup(MathTex('\\Vec{S}_{3}'),Dot(ax.c2p(7,3))).set_color(BLUE)

		#Placement of the labels
		s1[0].next_to(s1[1],UP)
		s2[0].next_to(s2[1],UP)
		s3[0].next_to(s3[1],UP)

		#Ordering label based on fitness of solution vectors
		orderingLabel = MathTex('F\\left(\\Vec{S}_{1}\\right)','<','F\\left(\\Vec{S}_{2}\\right)','<','F\\left(\\Vec{S}_{3}\\right)').scale(0.75)
		orderingLabel[0].set_color(RED)
		orderingLabel[2].set_color(GREEN)
		orderingLabel[4].set_color(BLUE)

		orderingLabel.to_corner(UR)

		#difference vector placement
		diff = Arrow(start=s2[1],end=s3[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1)

		s1Arrow = Arrow(start=ax.get_origin(),end=s1[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=RED)
		s2Arrow = Arrow(start=ax.get_origin(),end=s2[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=GREEN)
		s3Arrow = Arrow(start=ax.get_origin(),end=s3[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=BLUE)

		diffLabel = MathTex('\\Vec{D}','=','\\Vec{S}_{3}','-','\\Vec{S}_{2}').scale(0.75).next_to(orderingLabel,DOWN)
		#diffLabel[0].set_color(GREY)
		diffLabel[2].set_color(BLUE)
		diffLabel[4].set_color(GREEN)

		diffLabelWithF = MathTex('f','\\cdot','\\Vec{D}').move_to(diffLabel.get_center()).scale(0.75)

		newS1 = VGroup(MathTex('\\Vec{S}_{1}^{\\prime}'),Dot(ax.c2p(3.5,3))).set_color(ORANGE)
		newS1[0].next_to(newS1[1],UP)
		newS1Arrow = Arrow(start=ax.get_origin(),end=s1[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=ORANGE)

		newS1LabelEq = MathTex('\\Vec{S}_{1}^{\\prime}','=','\\Vec{S}_{1}','+','f','\\cdot','\\Vec{D}').move_to(diffLabel.get_center()).scale(0.75)
		newS1LabelEq[2].set_color(RED)
		newS1LabelEq[0].set_color(ORANGE)
		
		#Scene Creation
		#Adding axes to the scene
		self.play(Create(ax),run_time=2)
		self.wait()

		#Adding solution vectors to the scene
		self.play(FadeIn(VGroup(s1[1],s2[1],s3[1])),run_time=2)
		self.play(Write(VGroup(s1[0],s2[0],s3[0])),run_time=2)
		self.wait()

		self.play(Write(orderingLabel),run_time=2)
		self.wait()

		self.play(FadeIn(VGroup(s1Arrow,s2Arrow,s3Arrow)),run_time=2)
		self.wait()

		self.play(Write(diffLabel),run_time=2)
		self.play(Create(diff),run_time=2)
		self.wait()

		self.play(ReplacementTransform(diffLabel,diffLabelWithF),run_time=2)
		self.play(diff.animate.put_start_and_end_on(s2[1].get_center(),ax.c2p(6.5,5)),run_time=2)
		self.wait()

		self.play(ReplacementTransform(diffLabelWithF,newS1LabelEq),run_time=2)
		self.play(diff.animate.put_start_and_end_on(s1[1].get_center(),ax.c2p(3.5,3)),FadeOut(s2Arrow,s3Arrow),run_time=2)
		self.play(newS1Arrow.animate.put_start_and_end_on(ax.get_origin(),ax.c2p(3.5,3)),run_time=2)
		self.play(Write(newS1[0]),FadeIn(newS1[1]),run_time=2)
		self.play(FadeOut(diff),run_time=2)
		self.wait()

class Chromosome(Scene):
	def getChromosome(self,chromosome):
		resultChromosome=VGroup()
		resultChromosome.add(self.getBox(chromosome[0]))

		for i in range(1,len(chromosome)):
			box=self.getBox(chromosome[i])
			box.shift(RIGHT*i)
			resultChromosome.add(box)
		
		resultChromosome.move_to((0,0,0))
		
		return resultChromosome

	def getBox(self,text):
		result=VGroup()

		box=Square(side_length=1,color=GRAY)
		text=Text(str(text)).move_to(box.get_center())

		result.add(box,text)
		return result
	
	def getIndices(self,chromosome,startNum):
		indices=VGroup()

		for i in range(len(chromosome)):
			text=Text(str(i+startNum),font_size=20).next_to(chromosome[i],UP)
			indices.add(text)

		return indices

class DE_Crossover(Scene):
	def construct(self):
		#Mobjects initialization
		c1 = [6,3,3,2,4,3,6,3,6,4]
		c2 = [3,6,4,3,3,4,6,2,4,3]

		util = Chromosome()

		chromosome1 = util.getChromosome(c1).set_color(RED)
		chromosome2 = util.getChromosome(c2).set_color(BLUE)

		p1 = [' ',' ',' ',' ',' ',' ',' ',' ',' ',' ']
		parent1 = util.getChromosome(p1)


		#Animation
		self.play(VGroup(chromosome1,chromosome2).animate.arrange(DOWN),run_time=2)
		chromLabel1 = MathTex('C',':=').next_to(chromosome1,LEFT)
		chromLabel2 = MathTex('C^{\\prime}',':=').next_to(chromosome2,LEFT)
		self.play(Write(VGroup(chromLabel1,chromLabel2)),run_time=2)
		
		probs = [0.62,0.71,0.18,0.54,0.46,0.11,0.93,0.89,0.08,0.26]

		randNum = MathTex('r:=','0').next_to(chromosome1,UP)

		self.play(Write(randNum),run_time=2)
		self.wait()

		box = SurroundingRectangle(VGroup(chromosome1[0][0],chromosome2[0][0]))
		self.play(Create(box),run_time=1)
		self.play(Transform(randNum,MathTex('r:=',str(probs[0])).next_to(chromosome1,UP)),run_time=0.5)
		gene=chromosome1[0].copy().move_to(chromosome2[0].get_center())
		self.play(Transform(chromosome2[0],gene),run_time=1)
		for i in range(1,len(c1)):
			self.play(Transform(box,SurroundingRectangle(VGroup(chromosome1[i][0],chromosome2[i][0]))),run_time=1)
			self.play(Transform(randNum,MathTex('r:=',str(probs[i])).next_to(chromosome1,UP)),run_time=0.5)

			if probs[i]>0.4:
				gene=chromosome1[i].copy().move_to(chromosome2[i].get_center())
				self.play(Transform(chromosome2[i],gene),run_time=1)

		self.play(Uncreate(box),run_time=1)
		self.wait()

