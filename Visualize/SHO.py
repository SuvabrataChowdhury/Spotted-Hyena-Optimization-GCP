from manim import *

class SHO_Encircle(Scene):
	def construct(self):
		#Mobjects Initialization
		ax = Axes(x_range=[0,10],y_range=[0,10],tips=True)

		prey = VGroup(MathTex('\\Vec{P}_{p}'),Dot(ax.c2p(2,6),color=GREEN))
		prey[0].next_to(prey[1],UP)

		hyena = VGroup(MathTex('\\Vec{P}_{h}(x)'),Dot(ax.c2p(5,3),color=RED))
		hyena[0].next_to(hyena[1],UP)

		prey.add(Arrow(start=ax.c2p(0,0),end=prey[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=GREEN))
		hyena.add(Arrow(start=ax.c2p(0,0),end=hyena[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=RED))

		weightedPrey = VGroup( MathTex('\\Vec{B}','\\cdot','\\Vec{P}_{p}') , Dot(ax.c2p(1,5),color=BLUE) )
		weightedPrey[0].next_to(weightedPrey[1],UP)

		weightedPrey.add(Arrow(start=ax.c2p(0,0),end=prey[1],buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=BLUE))

		vecD = VGroup(MathTex('\\Vec{D}_{h}'),Dot(ax.c2p(4,2)))
		vecD[0].next_to(vecD[1],UP)

		vecD.add(Arrow(start=hyena[1].get_center(),end=weightedPrey[1].get_center(),buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1))

		newHyena = VGroup(MathTex('\\Vec{P}_{h}(x+1)'),Dot(ax.c2p(1,2),color=ORANGE))
		newHyena[0].next_to(newHyena[1],UR)
		
		newHyena.add(Arrow(start=ax.c2p(1,4),end=prey[1].get_center(),buff=0,stroke_width=3,max_tip_length_to_length_ratio=0.1,color=ORANGE))

		vecDEq = MathTex('\\Vec{P}_{p}').to_corner(UR)

		#MObjects Animation
		self.play(Create(ax),run_time=2)
		self.wait()

		self.play(FadeIn(VGroup(prey[1],hyena[1])),run_time=2)
		self.play(Write(VGroup(prey[0],hyena[0])),run_time=2)
		self.wait()

		self.play(FadeIn(VGroup(prey[2],hyena[2])),run_time=2)
		self.play(Write(vecDEq),run_time=2)
		self.wait()

		self.play(Transform(vecDEq,MathTex('\\Vec{B}','\\cdot','\\Vec{P}_{p}').to_corner(UR)),run_time=2)
		self.play(weightedPrey[2].animate.put_start_and_end_on(start=ax.c2p(0,0),end=ax.c2p(1,5)),run_time=2)
		self.play(FadeIn(weightedPrey[1]),Write(weightedPrey[0]),run_time=2)
		self.wait()

		self.play(Transform(vecDEq,MathTex('\\Vec{B}','\\cdot','\\Vec{P}_{p}','-','\\Vec{P}_{h}').to_corner(UR)),run_time=2)
		self.play(Create(vecD[2]),run_time=2)
		self.play(Transform(vecDEq,MathTex('\\Vec{D}_{h}','=','|\\Vec{B}','\\cdot','\\Vec{P}_{p}','-','\\Vec{P}_{h}|').to_corner(UR)),run_time=2)
		self.play(FadeOut(weightedPrey),vecD[2].animate.put_start_and_end_on(start=ax.c2p(0,0),end=vecD[1].get_center()),run_time=2)
		self.play(FadeIn(vecD[1]),Write(vecD[0]),run_time=2)
		self.wait()

		self.play(Transform(vecDEq,MathTex('\\Vec{E}','\\cdot','\\Vec{D}_{h}').to_corner(UR)),run_time=2)
		self.play(Transform(vecD[1],Dot(ax.c2p(1,4))),Transform(vecD[0],MathTex('\\Vec{E}','\\cdot','\\Vec{D}_{h}').next_to(ax.c2p(1,4),RIGHT)),vecD[2].animate.put_start_and_end_on(start=ax.c2p(0,0),end=ax.c2p(1,4)),run_time=2)
		self.wait()

		self.play(Transform(vecDEq,MathTex('\\Vec{P}_{h}(x+1)','=','\\Vec{P}_p','-','\\Vec{E}','\\cdot','\\Vec{D}_{h}').to_corner(UR)),run_time=2)
		self.play(Create(newHyena[2]),run_time=2)
		self.play(FadeOut(vecD),newHyena[2].animate.put_start_and_end_on(start=ax.c2p(0,0),end=newHyena[1].get_center()),run_time=2)
		self.play(FadeIn(newHyena[1]),Write(newHyena[0]),run_time=2)
		self.wait()
