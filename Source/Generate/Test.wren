import "soup" for Soup, SoupExtension

class TestClass is SoupExtension {
	static runBefore { ["OTher"] }
	static runAfter { ["A1", "B2"] }

	static evaluate() {
		Soup.debug("I am running in a method!")

		var activeState = Soup.activeState
		Soup.debug("hm %(activeState)")
		var sharedState = Soup.sharedState
		var sharedState2 = Soup.sharedState
		Soup.debug("hm2 %(sharedState)")

		activeState["Test"] = "test"
		sharedState2["dsdf"] = "huh"

		Soup.createOperation("test", "test", "test", "test", "test", "test")
	}
}