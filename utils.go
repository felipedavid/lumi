package main

func isDigit(char byte) bool {
	return char >= '0' && char <= '9'
}

func isAlpha(char byte) bool {
	return (char >= 'A' && char <= 'Z') || (char >= 'a' && char <= 'z')
}

func isAlphaNumeric(char byte) bool {
	return isDigit(char) || isAlpha(char)
}
